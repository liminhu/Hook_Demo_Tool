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
LOCAL_MODULE    := pub_tool_hook
#一定要有.cy作后缀
LOCAL_SRC_FILES := pub_tool_hook.cy.cpp
LOCAL_LDLIBS := -llog
LOCAL_ARM_MODE := arm
LOCAL_LDLIBS += -L$(LOCAL_PATH) -lsubstrate-dvm -lsubstrate
LOCAL_CFLAGS += -Wpointer-arith -fpermissive -Wformat
include $(BUILD_SHARED_LIBRARY)




##include $(BUILD_SHARED_LIBRARY)
##LOCAL_MODULE    := pub_tool
##LOCAL_SRC_FILES := pub_tool_hook.cy.cpp substrate.h TKHooklib.h
##LOCAL_LDLIBS:= -L$(LOCAL_PATH) -lsubstrate -lsubstrate-dvm -llog


