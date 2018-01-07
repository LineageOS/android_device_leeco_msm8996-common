LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    CameraModule.cpp \
    CameraMetadata.cpp \
    CameraParameters.cpp \
    VendorTagDescriptor.cpp \
    HandleImporter.cpp

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/include \
    system/media/private/camera/include

LOCAL_SHARED_LIBRARIES := \
    liblog \
    libhardware \
    libcamera_metadata \
    libcutils \
    android.hardware.graphics.mapper@2.0

LOCAL_MODULE := android.hardware.camera.common@1.0-helper-custom
LOCAL_MODULE_TAGS := optional
LOCAL_VENDOR_MODULE := true

include $(BUILD_STATIC_LIBRARY)
