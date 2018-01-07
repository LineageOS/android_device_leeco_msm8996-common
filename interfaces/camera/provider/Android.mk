LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
    CameraProvider.cpp

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/../helper/include

LOCAL_SHARED_LIBRARIES := \
    libhidlbase \
    libhidltransport \
    libutils \
    libcutils \
    android.hardware.camera.device@1.0 \
    android.hardware.camera.device@3.2 \
    android.hardware.camera.device@3.3 \
    camera.device@1.0-impl \
    camera.device@3.2-impl \
    camera.device@3.3-impl \
    android.hardware.camera.provider@2.4 \
    android.hardware.camera.common@1.0 \
    android.hardware.graphics.mapper@2.0 \
    android.hidl.allocator@1.0 \
    android.hidl.memory@1.0 \
    liblog \
    libhardware \
    libcamera_metadata

LOCAL_STATIC_LIBRARIES := \
    android.hardware.camera.common@1.0-helper-custom

LOCAL_MODULE := android.hardware.camera.provider@2.4-impl-custom
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_VENDOR_MODULE := true

include $(BUILD_SHARED_LIBRARY)
