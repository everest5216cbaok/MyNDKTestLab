LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)
#local
LOCAL_MODULE:=game_static
#
MY_SRC_BASE_PATH:=../src/
MY_INC_BASE_PATH:=../inc/
MY_INC_OUT_BASE_PATH:=../../gles/
#src setting
MY_SRC_FILES:=\
$(MY_SRC_BASE_PATH)/game.cpp
LOCAL_SRC_FILES:=$(MY_SRC_FILES)
#inc setting
MY_C_INCLUDES:=\
$(LOCAL_PATH)/$(MY_INC_BASE_PATH) \
$(LOCAL_PATH)/$(MY_INC_OUT_BASE_PATH) \
$(LOCAL_PATH)/../../matrix_calc/inc \
$(LOCAL_PATH)/../../ogldev_tools/inc
LOCAL_EXPORT_C_INCLUDES:=$(MY_C_INCLUDES)
LOCAL_C_INCLUDES:=$(MY_C_INCLUDES)

LOCAL_CFLAGS += -Wno-psabi
LOCAL_EXPORT_CFLAGS += -Wno-psabi

LOCAL_LDLIBS:=-lGLESv2

LOCAL_WHOLE_STATIC_LIBRARIES := matrix_calc_static ogldev_tools_static

include $(BUILD_STATIC_LIBRARY)

$(call import-module,matrix_calc/android)
$(call import-module,ogldev_tools/android)