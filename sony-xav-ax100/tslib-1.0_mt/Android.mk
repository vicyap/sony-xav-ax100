LOCAL_PATH := $(call my-dir)
MLOCAL_PATH := $(LOCAL_PATH)

include $(CLEAR_VARS)

LOCAL_MODULE := tslib-1.0

LOCAL_C_INCLUDES := $(LOCAL_PATH) $(LOCAL_PATH)/plugins $(LOCAL_PATH)/src
LOCAL_CFLAGS := -O3 -DTS_CONF="\"/system/usr/local/ts/etc/ts.conf\"" \
						-DPLUGIN_DIR="\"/system/usr/local/ts/lib/ts\""

LOCAL_CPP_EXTENSION := .cpp

#LOCAL_SRC_FILES := $(notdir $(wildcard $(LOCAL_PATH)/src/*.c))

LOCAL_SRC_FILES := src/ts_attach.c src/ts_close.c src/ts_config.c src/ts_error.c src/ts_fd.c src/ts_load_module.c  src/ts_open.c  src/ts_parse_vars.c src/ts_read.c  src/ts_read_raw.c src/ts_read_raw_module.c 

#LOCAL_STATIC_LIBRARIES := png jpeg
#LOCAL_SHARED_LIBRARIES := sdl-$(SDL_VERSION)
#LOCAL_LDLIBS := -lz

include $(BUILD_SHARED_LIBRARY)
include $(MLOCAL_PATH)/plugins/Android.mk

include $(MLOCAL_PATH)/tests/Android.mk

#include $(call all-subdir-makefiles)



