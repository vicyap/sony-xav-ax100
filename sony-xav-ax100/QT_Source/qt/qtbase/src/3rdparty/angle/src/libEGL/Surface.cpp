#include "../libGLESv2/precompiled.h"
//
// Copyright (c) 2002-2012 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

// Surface.cpp: Implements the egl::Surface class, representing a drawing surface
// such as the client area of a window, including any back buffers.
// Implements EGLSurface and related functionality. [EGL 1.4] section 2.2 page 3.

#include <tchar.h>

#include <algorithm>

#include "libEGL/Surface.h"

#include "common/debug.h"
#include "libGLESv2/Texture.h"
#include "libGLESv2/renderer/SwapChain.h"
#include "libGLESv2/main.h"

#include "libEGL/main.h"
#include "libEGL/Display.h"

#if defined(ANGLE_OS_WINRT)
#include <wrl.h>
#include <windows.foundation.h>
#include <windows.ui.core.h>
#include <windows.graphics.display.h>

static bool getCoreWindowSize(const EGLNativeWindowType win, int *width, int *height)
{
    Microsoft::WRL::ComPtr<ABI::Windows::UI::Core::ICoreWindow> window;
    HRESULT hr = win->QueryInterface(IID_PPV_ARGS(&window));
    if (FAILED(hr))
    {
        ERR("Failed to cast native display pointer to ICoreWindow *.");
        return false;
    }

#if _MSC_VER<=1700
    Microsoft::WRL::ComPtr<ABI::Windows::Graphics::Display::IDisplayPropertiesStatics> displayInformation;
    hr = RoGetActivationFactory(Microsoft::WRL::Wrappers::HString::MakeReference(RuntimeClass_Windows_Graphics_Display_DisplayProperties).Get(),
                                IID_PPV_ARGS(&displayInformation));
#else
    Microsoft::WRL::ComPtr<ABI::Windows::Graphics::Display::IDisplayInformationStatics> displayInformationFactory;
    hr = RoGetActivationFactory(Microsoft::WRL::Wrappers::HString::MakeReference(RuntimeClass_Windows_Graphics_Display_DisplayInformation).Get(),
                                IID_PPV_ARGS(&displayInformationFactory));
    if (FAILED(hr))
    {
        ERR("Failed to get display information factory.");
        return false;
    }

    Microsoft::WRL::ComPtr<ABI::Windows::Graphics::Display::IDisplayInformation> displayInformation;
    hr = displayInformationFactory->GetForCurrentView(&displayInformation);
#endif
    if (FAILED(hr))
    {
        ERR("Failed to get display information.");
        return false;
    }

#if defined(ANGLE_OS_WINPHONE) && _MSC_VER>=1800 // Windows Phone 8.1
    Microsoft::WRL::ComPtr<ABI::Windows::Graphics::Display::IDisplayInformation2> displayInformation2;
    hr = displayInformation.As(&displayInformation2);
    if (FAILED(hr))
    {
        ERR("Failed to get extended display information.");
        return false;
    }

    DOUBLE scaleFactor;
    hr = displayInformation2->get_RawPixelsPerViewPixel(&scaleFactor);
    if (FAILED(hr))
    {
        ERR("Failed to get raw pixels per view pixel.");
        return false;
    }
#else
    ABI::Windows::Graphics::Display::ResolutionScale resolutionScale;
    hr = displayInformation->get_ResolutionScale(&resolutionScale);
    if (FAILED(hr))
    {
        ERR("Failed to get resolution scale.");
        return false;
    }
    DOUBLE scaleFactor = DOUBLE(resolutionScale) / 100.0;
#endif

    ABI::Windows::Foundation::Rect windowRect;
    hr = window->get_Bounds(&windowRect);
    if (FAILED(hr))
    {
        ERR("Failed to get ICoreWindow bounds.");
        return false;
    }

    *width = std::floor(windowRect.Width * scaleFactor + 0.5);
    *height = std::floor(windowRect.Height * scaleFactor + 0.5);
    return true;
}
#endif

namespace egl
{

Surface::Surface(Display *display, const Config *config, EGLNativeWindowType window, EGLint postSubBufferSupported)
    : mDisplay(display), mConfig(config), mWindow(window), mPostSubBufferSupported(postSubBufferSupported)
{
    mRenderer = mDisplay->getRenderer();
    mSwapChain = NULL;
    mShareHandle = NULL;
    mTexture = NULL;
    mTextureFormat = EGL_NO_TEXTURE;
    mTextureTarget = EGL_NO_TEXTURE;

    mPixelAspectRatio = (EGLint)(1.0 * EGL_DISPLAY_SCALING);   // FIXME: Determine actual pixel aspect ratio
    mRenderBuffer = EGL_BACK_BUFFER;
    mSwapBehavior = EGL_BUFFER_PRESERVED;
    mSwapInterval = -1;
    mWidth = -1;
    mHeight = -1;
    setSwapInterval(1);

    subclassWindow();
}

Surface::Surface(Display *display, const Config *config, HANDLE shareHandle, EGLint width, EGLint height, EGLenum textureFormat, EGLenum textureType)
    : mDisplay(display), mWindow(NULL), mConfig(config), mShareHandle(shareHandle), mWidth(width), mHeight(height), mPostSubBufferSupported(EGL_FALSE)
{
    mRenderer = mDisplay->getRenderer();
    mSwapChain = NULL;
    mWindowSubclassed = false;
    mTexture = NULL;
    mTextureFormat = textureFormat;
    mTextureTarget = textureType;

    mPixelAspectRatio = (EGLint)(1.0 * EGL_DISPLAY_SCALING);   // FIXME: Determine actual pixel aspect ratio
    mRenderBuffer = EGL_BACK_BUFFER;
    mSwapBehavior = EGL_BUFFER_PRESERVED;
    mSwapInterval = -1;
    setSwapInterval(1);
}

Surface::~Surface()
{
    unsubclassWindow();
    release();
}

bool Surface::initialize()
{
    if (!resetSwapChain())
      return false;

    return true;
}

void Surface::release()
{
    delete mSwapChain;
    mSwapChain = NULL;

    if (mTexture)
    {
        mTexture->releaseTexImage();
        mTexture = NULL;
    }
}

bool Surface::resetSwapChain()
{
    ASSERT(!mSwapChain);

    int width;
    int height;

    if (mWindow)
    {
#if !defined(ANGLE_OS_WINRT)
        RECT windowRect;
        if (!GetClientRect(getWindowHandle(), &windowRect))
        {
            ASSERT(false);

            ERR("Could not retrieve the window dimensions");
            return error(EGL_BAD_SURFACE, false);
        }

        width = windowRect.right - windowRect.left;
        height = windowRect.bottom - windowRect.top;
#else
        if (!getCoreWindowSize(mWindow, &width, &height))
        {
            return false;
        }
#endif
    }
    else
    {
        // non-window surface - size is determined at creation
        width = mWidth;
        height = mHeight;
    }

    mSwapChain = mRenderer->createSwapChain(mWindow, mShareHandle,
                                            mConfig->mRenderTargetFormat,
                                            mConfig->mDepthStencilFormat);
    if (!mSwapChain)
    {
        return error(EGL_BAD_ALLOC, false);
    }

    if (!resetSwapChain(width, height))
    {
        delete mSwapChain;
        mSwapChain = NULL;
        return false;
    }

    return true;
}

bool Surface::resizeSwapChain(int backbufferWidth, int backbufferHeight)
{
    ASSERT(backbufferWidth >= 0 && backbufferHeight >= 0);
    ASSERT(mSwapChain);

    EGLint status = mSwapChain->resize(backbufferWidth, backbufferHeight);

    if (status == EGL_CONTEXT_LOST)
    {
        mDisplay->notifyDeviceLost();
        return false;
    }
    else if (status != EGL_SUCCESS)
    {
        return error(status, false);
    }

    mWidth = backbufferWidth;
    mHeight = backbufferHeight;

    return true;
}

bool Surface::resetSwapChain(int backbufferWidth, int backbufferHeight)
{
    ASSERT(backbufferWidth >= 0 && backbufferHeight >= 0);
    ASSERT(mSwapChain);

    EGLint status = mSwapChain->reset(backbufferWidth, backbufferHeight, mSwapInterval);

    if (status == EGL_CONTEXT_LOST)
    {
        mRenderer->notifyDeviceLost();
        return false;
    }
    else if (status != EGL_SUCCESS)
    {
        return error(status, false);
    }

    mWidth = backbufferWidth;
    mHeight = backbufferHeight;
    mSwapIntervalDirty = false;

    return true;
}

bool Surface::swapRect(EGLint x, EGLint y, EGLint width, EGLint height)
{
    if (!mSwapChain)
    {
        return true;
    }

    if (x + width > mWidth)
    {
        width = mWidth - x;
    }

    if (y + height > mHeight)
    {
        height = mHeight - y;
    }

    if (width == 0 || height == 0)
    {
        return true;
    }

    EGLint status = mSwapChain->swapRect(x, y, width, height);

    if (status == EGL_CONTEXT_LOST)
    {
        mRenderer->notifyDeviceLost();
        return false;
    }
    else if (status != EGL_SUCCESS)
    {
        return error(status, false);
    }

    checkForOutOfDateSwapChain();

    return true;
}

EGLNativeWindowType Surface::getWindowHandle()
{
    return mWindow;
}


#define kSurfaceProperty _TEXT("Egl::SurfaceOwner")
#define kParentWndProc _TEXT("Egl::SurfaceParentWndProc")

#if !defined(ANGLE_OS_WINRT)
static LRESULT CALLBACK SurfaceWindowProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
  if (message == WM_SIZE)
  {
      Surface* surf = reinterpret_cast<Surface*>(GetProp(hwnd, kSurfaceProperty));
      if(surf)
      {
          surf->checkForOutOfDateSwapChain();
      }
  }
  WNDPROC prevWndFunc = reinterpret_cast<WNDPROC >(GetProp(hwnd, kParentWndProc));
  return CallWindowProc(prevWndFunc, hwnd, message, wparam, lparam);
}
#endif

void Surface::subclassWindow()
{
#if defined(ANGLE_OS_WINRT)
    mWindowSubclassed = false;
#else
    if (!mWindow)
    {
        return;
    }

    DWORD processId;
    DWORD threadId = GetWindowThreadProcessId(mWindow, &processId);
    if (processId != GetCurrentProcessId() || threadId != GetCurrentThreadId())
    {
        return;
    }

    SetLastError(0);
    LONG_PTR oldWndProc = SetWindowLongPtr(mWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(SurfaceWindowProc));
    if(oldWndProc == 0 && GetLastError() != ERROR_SUCCESS)
    {
        mWindowSubclassed = false;
        return;
    }

    SetProp(mWindow, kSurfaceProperty, reinterpret_cast<HANDLE>(this));
    SetProp(mWindow, kParentWndProc, reinterpret_cast<HANDLE>(oldWndProc));
    mWindowSubclassed = true;
#endif
}

void Surface::unsubclassWindow()
{
#if !defined(ANGLE_OS_WINRT)
    if(!mWindowSubclassed)
    {
        return;
    }

    // un-subclass
    LONG_PTR parentWndFunc = reinterpret_cast<LONG_PTR>(GetProp(mWindow, kParentWndProc));

    // Check the windowproc is still SurfaceWindowProc.
    // If this assert fails, then it is likely the application has subclassed the
    // hwnd as well and did not unsubclass before destroying its EGL context. The
    // application should be modified to either subclass before initializing the
    // EGL context, or to unsubclass before destroying the EGL context.
    if(parentWndFunc)
    {
        LONG_PTR prevWndFunc = SetWindowLongPtr(mWindow, GWLP_WNDPROC, parentWndFunc);
        ASSERT(prevWndFunc == reinterpret_cast<LONG_PTR>(SurfaceWindowProc));
    }

    RemoveProp(mWindow, kSurfaceProperty);
    RemoveProp(mWindow, kParentWndProc);
    mWindowSubclassed = false;
#endif
}

bool Surface::checkForOutOfDateSwapChain()
{
#if !defined(ANGLE_OS_WINRT)
    RECT client;
    if (!GetClientRect(getWindowHandle(), &client))
    {
        ASSERT(false);
        return false;
    }

    // Grow the buffer now, if the window has grown. We need to grow now to avoid losing information.
    int clientWidth = client.right - client.left;
    int clientHeight = client.bottom - client.top;
#else
    int clientWidth;
    int clientHeight;
    if (!getCoreWindowSize(mWindow, &clientWidth, &clientHeight))
    {
        return false;
    }
#endif
    bool sizeDirty = clientWidth != getWidth() || clientHeight != getHeight();

#if !defined(ANGLE_OS_WINRT)
    if (IsIconic(getWindowHandle()))
    {
        // The window is automatically resized to 150x22 when it's minimized, but the swapchain shouldn't be resized
        // because that's not a useful size to render to.
        sizeDirty = false;
    }
#endif

    bool wasDirty = (mSwapIntervalDirty || sizeDirty);

    if (mSwapIntervalDirty)
    {
        resetSwapChain(clientWidth, clientHeight);
    }
    else if (sizeDirty)
    {
        resizeSwapChain(clientWidth, clientHeight);
    }

    if (wasDirty)
    {
        if (static_cast<egl::Surface*>(getCurrentDrawSurface()) == this)
        {
            glMakeCurrent(glGetCurrentContext(), static_cast<egl::Display*>(getCurrentDisplay()), this);
        }

        return true;
    }

    return false;
}

bool Surface::swap()
{
    return swapRect(0, 0, mWidth, mHeight);
}

bool Surface::postSubBuffer(EGLint x, EGLint y, EGLint width, EGLint height)
{
    if (!mPostSubBufferSupported)
    {
        // Spec is not clear about how this should be handled.
        return true;
    }
    
    return swapRect(x, y, width, height);
}

EGLint Surface::getWidth() const
{
    return mWidth;
}

EGLint Surface::getHeight() const
{
    return mHeight;
}

EGLint Surface::isPostSubBufferSupported() const
{
    return mPostSubBufferSupported;
}

rx::SwapChain *Surface::getSwapChain() const
{
    return mSwapChain;
}

void Surface::setSwapInterval(EGLint interval)
{
    if (mSwapInterval == interval)
    {
        return;
    }
    
    mSwapInterval = interval;
    mSwapInterval = std::max(mSwapInterval, mRenderer->getMinSwapInterval());
    mSwapInterval = std::min(mSwapInterval, mRenderer->getMaxSwapInterval());

    mSwapIntervalDirty = true;
}

EGLenum Surface::getTextureFormat() const
{
    return mTextureFormat;
}

EGLenum Surface::getTextureTarget() const
{
    return mTextureTarget;
}

void Surface::setBoundTexture(gl::Texture2D *texture)
{
    mTexture = texture;
}

gl::Texture2D *Surface::getBoundTexture() const
{
    return mTexture;
}

EGLenum Surface::getFormat() const
{
    return mConfig->mRenderTargetFormat;
}
}
