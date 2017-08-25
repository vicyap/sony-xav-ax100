LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= input-raw.c

LOCAL_C_INCLUDES += \
        $(LOCAL_PATH)/../ \
        $(LOCAL_PATH)/../src

LOCAL_SHARED_LIBRARIES := tslib-1.0
LOCAL_MODULE := input
LOCAL_PRELINK_MODULE := false
include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_SRC_FILES:= mtinput-raw.c

LOCAL_C_INCLUDES += \
        $(LOCAL_PATH)/../ \
        $(LOCAL_PATH)/../src

LOCAL_SHARED_LIBRARIES := tslib-1.0
LOCAL_MODULE := mtinput
LOCAL_PRELINK_MODULE := false
include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_SRC_FILES:= pthres.c

LOCAL_C_INCLUDES += \
        $(LOCAL_PATH)/../ \
        $(LOCAL_PATH)/../src

LOCAL_SHARED_LIBRARIES := tslib-1.0
LOCAL_MODULE := pthres
LOCAL_PRELINK_MODULE := false
include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_SRC_FILES:= variance.c

LOCAL_C_INCLUDES += \
        $(LOCAL_PATH)/../ \
        $(LOCAL_PATH)/../src

LOCAL_SHARED_LIBRARIES := tslib-1.0
LOCAL_MODULE := variance
LOCAL_PRELINK_MODULE := false
include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_SRC_FILES:= dejitter.c

LOCAL_C_INCLUDES += \
        $(LOCAL_PATH)/../ \
        $(LOCAL_PATH)/../src

LOCAL_SHARED_LIBRARIES := tslib-1.0
LOCAL_MODULE := dejitter
LOCAL_PRELINK_MODULE := false
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES:= linear.c

LOCAL_C_INCLUDES += \
        $(LOCAL_PATH)/../ \
        $(LOCAL_PATH)/../src

LOCAL_SHARED_LIBRARIES := tslib-1.0
LOCAL_MODULE := linear
LOCAL_PRELINK_MODULE := false
include $(BUILD_SHARED_LIBRARY)

