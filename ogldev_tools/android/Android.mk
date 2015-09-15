LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE:=ogldev_tools_static

MY_SRC_BASE_PATH:=../src/
MY_INC_BASE_PATH:=../inc/
MY_INC_OUT_BASE_PATH:=../../gles
MY_INC_OUT_PATH:=../../assimp/ #assimpÂ·¾¶

#FILE_LIST:=$(wildcard $(LOCAL_PATH)/$(MY_SRC_BASE_PATH)/*.cpp)
#LOCAL_SRC_FILES:=$(FILE_LIST:$(LOCAL_PATH)/%=%)

MY_SRC_FILES:=\
$(MY_SRC_BASE_PATH)/camera.cpp \
$(MY_SRC_BASE_PATH)/lodepng.cpp \
$(MY_SRC_BASE_PATH)/math_3d.cpp \
$(MY_SRC_BASE_PATH)/ogldev_basic_lighting.cpp \
$(MY_SRC_BASE_PATH)/ogldev_texture.cpp \
$(MY_SRC_BASE_PATH)/ogldev_util.cpp \
$(MY_SRC_BASE_PATH)/pipeline.cpp \
$(MY_SRC_BASE_PATH)/technique.cpp
LOCAL_SRC_FILES:=$(MY_SRC_FILES)

LOCAL_C_INCLUDES:= $(LOCAL_PATH)/$(MY_INC_OUT_BASE_PATH) \
$(LOCAL_PATH)/$(MY_INC_BASE_PATH) \
$(LOCAL_PATH)/$(MY_INC_OUT_PATH)

LOCAL_EXPORT_C_INCLUDES:= $(LOCAL_PATH)/$(MY_INC_OUT_BASE_PATH) \
$(LOCAL_PATH)/$(MY_INC_BASE_PATH) \
$(LOCAL_PATH)/$(MY_INC_OUT_PATH)


LOCAL_CFLAGS += -Wno-psabi
LOCAL_EXPORT_CFLAGS += -Wno-psabi

include $(BUILD_STATIC_LIBRARY)