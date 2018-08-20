LOCAL_PATH := $(call my-dir)
 
include $(CLEAR_VARS)
LOCAL_MODULE:= substrate-dvm
LOCAL_SRC_FILES := libsubstrate-dvm.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE:= substrate
LOCAL_SRC_FILES := libsubstrate.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := TKHooklib
LOCAL_SRC_FILES := libTKHooklib.so
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := test_so_hook.cy
LOCAL_SRC_FILES := test_so_hook.cpp

##LOCAL_MODULE    := pub_tool_hook.cy
##LOCAL_SRC_FILES := pub_tool_hook.cpp

LOCAL_LDLIBS:= -L$(LOCAL_PATH) -lsubstrate -lsubstrate-dvm -llog 
LOCAL_SHARED_LIBRARIES := \
	libdl 
LOCAL_CFLAGS += -Wpointer-arith -fpermissive -Wformat
include $(BUILD_SHARED_LIBRARY)