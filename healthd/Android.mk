LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := healthd_board_msm.cpp healthd_msm_alarm.cpp
LOCAL_MODULE := libhealthd.msm

LOCAL_CFLAGS := -Werror
LOCAL_C_INCLUDES := \
    system/core/healthd/include/healthd/ \
    system/core/base/include \
    bootable/recovery \
    bootable/recovery/minui/include

include $(BUILD_STATIC_LIBRARY)
