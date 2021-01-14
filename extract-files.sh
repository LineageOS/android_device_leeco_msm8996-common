#!/bin/bash
#
# Copyright (C) 2016 The CyanogenMod Project
# Copyright (C) 2017-2020 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

set -e

# Load extract_utils and do some sanity checks
MY_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "${MY_DIR}" ]]; then MY_DIR="${PWD}"; fi

ANDROID_ROOT="${MY_DIR}/../../.."

HELPER="${ANDROID_ROOT}/tools/extract-utils/extract_utils.sh"
if [ ! -f "${HELPER}" ]; then
    echo "Unable to find helper script at ${HELPER}"
    exit 1
fi
source "${HELPER}"

# Default to sanitizing the vendor folder before extraction
CLEAN_VENDOR=true

ONLY_COMMON=
ONLY_TARGET=
KANG=
SECTION=

while [ "${#}" -gt 0 ]; do
    case "${1}" in
        --only-common )
                ONLY_COMMON=true
                ;;
        --only-target )
                ONLY_TARGET=true
                ;;
        -n | --no-cleanup )
                CLEAN_VENDOR=false
                ;;
        -k | --kang )
                KANG="--kang"
                ;;
        -s | --section )
                SECTION="${2}"; shift
                CLEAN_VENDOR=false
                ;;
        * )
                SRC="${1}"
                ;;
    esac
    shift
done

if [ -z "${SRC}" ]; then
    SRC="adb"
fi

function blob_fixup() {
    case "${1}" in

    # Move telephony packages to /system_ext
    system_ext/etc/init/dpmd.rc)
        sed -i "s/\/system\/product\/bin\//\/system\/system_ext\/bin\//g" "${2}"
        ;;

    # Move telephony packages to /system_ext
    system_ext/etc/permissions/com.qti.dpmframework.xml|system_ext/etc/permissions/dpmapi.xml|system_ext/etc/permissions/telephonyservice.xml)
        sed -i "s/\/system\/product\/framework\//\/system\/system_ext\/framework\//g" "${2}"
        ;;

    # Move telephony packages to /system_ext
    system_ext/etc/permissions/qcrilhook.xml)
        sed -i "s/\/product\/framework\//\/system\/system_ext\/framework\//g" "${2}"
        ;;

    # Provide shim for libdpmframework.so
    system_ext/lib64/libdpmframework.so)
        for  LIBCUTILS_SHIM in $(grep -L "libcutils_shim.so" "${2}"); do
            "${PATCHELF}" --add-needed "libcutils_shim.so" "$LIBCUTILS_SHIM"
        done
        ;;

    # kang vulkan from LA.UM.8.6.r1-01900-89xx.0
    vendor/lib/hw/vulkan.msm8996.so | vendor/lib64/hw/vulkan.msm8996.so)
        sed -i -e 's|vulkan.msm8953.so|vulkan.msm8996.so|g' "${2}"
        ;;

    # Patch blobs for VNDK
    vendor/lib/libmmcamera2_stats_modules.so)
        "${PATCHELF}" --remove-needed "libgui.so" "${2}"
        sed -i "s|/data/misc/camera|/data/vendor/qcam|g" "${2}"
        sed -i "s|libandroid.so|libcamshim.so|g" "${2}"
        ;;

    # Patch blobs for VNDK
    vendor/lib/libmmcamera_ppeiscore.so | vendor/lib/libcamera_letv_algo.so)
        "${PATCHELF}" --remove-needed "libgui.so" "${2}"
        ;;

    # Patch blobs for VNDK
    vendor/lib/libarcsoft_hdr_detection.so | vendor/lib/libmpbase.so | vendor/lib/libarcsoft_panorama_burstcapture.so | vendor/lib/libarcsoft_smart_denoise.so | vendor/lib/libarcsoft_nighthawk.so | vendor/lib/libarcsoft_hdr.so | vendor/lib/libarcsoft_night_shot.so)
        "${PATCHELF}" --remove-needed "libandroid.so" "${2}"
        ;;

    # Patch blobs for VNDK
    vendor/lib/libletv_algo_jni.so)
        "${PATCHELF}" --remove-needed "libgui.so" "${2}"
        "${PATCHELF}" --remove-needed "libandroid_runtime.so" "${2}"
        ;;

    # Patch blobs for VNDK
    vendor/lib64/lib-dplmedia.so)
        "${PATCHELF}" --remove-needed "libmedia.so" "${2}"
        ;;

    # Add shim for libbase LogMessage functions
    vendor/bin/imsrcsd | vendor/lib64/lib-uceservice.so)
        for  LIBBASE_SHIM in $(grep -L "libbase_shim.so" "${2}"); do
            "${PATCHELF}" --add-needed "libbase_shim.so" "$LIBBASE_SHIM"
        done
        ;;

    # Move ims libs to product
    product/etc/permissions/com.qualcomm.qti.imscmservice.xml)
        sed -i -e 's|file="/system/framework/|file="/product/framework/|g' "${2}"
        ;;

    # Move qti-vzw-ims-internal permission to vendor
    vendor/etc/permissions/qti-vzw-ims-internal.xml)
        sed -i -e 's|file="/system/vendor/|file="/vendor/|g' "${2}"
        ;;

    esac
}

if [ -z "${ONLY_TARGET}" ]; then
    # Initialize the helper for common device
    setup_vendor "${DEVICE_COMMON}" "${VENDOR}" "${ANDROID_ROOT}" true "${CLEAN_VENDOR}"

    extract "${MY_DIR}/proprietary-files.txt" "${SRC}" "${KANG}" --section "${SECTION}"
fi

if [ -z "${ONLY_COMMON}" ] && [ -s "${MY_DIR}/../${DEVICE}/proprietary-files.txt" ]; then
    # Reinitialize the helper for device
    source "${MY_DIR}/../${DEVICE}/extract-files.sh"
    setup_vendor "${DEVICE}" "${VENDOR}" "${ANDROID_ROOT}" false "${CLEAN_VENDOR}"

    extract "${MY_DIR}/../${DEVICE}/proprietary-files.txt" "${SRC}" "${KANG}" --section "${SECTION}"
fi

"${MY_DIR}/setup-makefiles.sh"
