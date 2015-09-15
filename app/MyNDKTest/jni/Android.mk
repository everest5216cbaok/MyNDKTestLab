LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE:=game
LOCAL_CFLAGS:=-Wall -Wextra
APP_SRC_PATH:=../../src
APP_SRC_FILES:=\
$(APP_SRC_PATH)/jni.c
LOCAL_SRC_FILES:=$(APP_SRC_FILES)
LOCAL_C_INCLUDES:=\
$(LOCAL_PATH)/$(APP_SRC_PATH)
LOCAL_LDLIBS:=-lGLESv2 -llog
LOCAL_CFLAGS := \
-DAPP_PLATFORM_ANDROID \
-DDEBUG
LOCAL_WHOLE_STATIC_LIBRARIES:=game_static matrix_calc_static ogldev_tools_static
include $(BUILD_SHARED_LIBRARY)
$(call import-module,face/android)
$(call import-module,matrix_calc/android)
$(call import-module,ogldev_tools/android)