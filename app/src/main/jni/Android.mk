LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# OpenCV
OPENCV_CAMERA_MODULES:=off
OPENCV_INSTALL_MODULES:=on

include ./src/main/jni/OpenCV-4.9.0-android-sdk/sdk/native/jni/OpenCV.mk

LOCAL_LDLIBS := -llog
LOCAL_SRC_FILES := ImageProcessing.cpp
LOCAL_MODULE    := ImageProcessing

include $(BUILD_SHARED_LIBRARY)
