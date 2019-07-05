#!/bin/bash
#
# Copyright (C) 2016 The CyanogenMod Project
# Copyright (C) 2017-2019 The LineageOS Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

set -e

# Load extract_utils and do some sanity checks
MY_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "${MY_DIR}" ]]; then MY_DIR="${PWD}"; fi

LINEAGE_ROOT="${MY_DIR}"/../../..

HELPER="${LINEAGE_ROOT}/vendor/lineage/build/tools/extract_utils.sh"
if [ ! -f "${HELPER}" ]; then
	echo "Unable to find helper script at $HELPER"
	exit 1
fi
source "${HELPER}"

# Default to sanitizing the vendor folder before extraction
CLEAN_VENDOR=true
KANG=

while [ "$#" -gt 0 ]; do
	case "$1" in
	-n|--no-cleanup)
		CLEAN_VENDOR=false
		;;
	-k|--kang)
		KANG="--kang"
		;;
	-s|--section)
		SECTION="$2"; shift
		CLEAN_VENDOR=false
		;;
	*)
		SRC="$1"
		;;
	esac
	shift
done

if [ -z "$SRC" ]; then
	SRC=adb
fi

function blob_fixup() {
	case "${1}" in

	# Correct android.hidl.manager@1.0-java jar name
	vendor/etc/permissions/qti_libpermissions.xml)
		sed -i -e 's|name=\"android.hidl.manager-V1.0-java|name=\"android.hidl.manager@1.0-java|g' "${2}"
		;;

	# Hax libaudcal.so to store acdbdata in new path
	vendor/lib/libaudcal.so | vendor/lib64/libaudcal.so)
		sed -i -e 's|\/data\/vendor\/misc\/audio\/acdbdata\/delta\/|\/data\/vendor\/audio\/acdbdata\/delta\/\x00\x00\x00\x00\x00|g' "${2}"
		;;

        vendor/lib64/hw/android.hardware.keymaster@3.0-impl.so|vendor/lib64/libsoftkeymasterdevice-v27.so|vendor/lib64/libkeymaster_messages-v27.so|vendor/lib64/libkeymaster_portable-v27.so|vendor/lib64/libkeymaster_staging-v27.so|vendor/lib64/libsoftkeymaster-v27.so)
                patchelf --replace-needed "libsoftkeymasterdevice.so" "libsoftkeymasterdevice-v27.so" "${2}"
                patchelf --replace-needed "libkeymaster_messages.so" "libkeymaster_messages-v27.so" "${2}"
                patchelf --replace-needed "libkeymaster_portable.so" "libkeymaster_portable-v27.so" "${2}"
                patchelf --replace-needed "libkeymaster_staging.so" "libkeymaster_staging-v27.so" "${2}"
                patchelf --replace-needed "libsoftkeymaster.so" "libsoftkeymaster-v27.so" "${2}"
                patchelf --set-soname $(basename "${2}") "${2}"
		;;
	esac
}

# Initialize the helper for common device
setup_vendor "${DEVICE_COMMON}" "${VENDOR}" "${LINEAGE_ROOT}" true "${CLEAN_VENDOR}"

extract "${MY_DIR}/proprietary-files.txt" "${SRC}" \
        "${KANG}" --section "${SECTION}"

if [ -s "${MY_DIR}/../${DEVICE}/proprietary-files.txt" ]; then
    # Reinitialize the helper for device
    source "${MY_DIR}/../${DEVICE}/extract-files.sh"
    setup_vendor "${DEVICE}" "${VENDOR}" "${LINEAGE_ROOT}" false "${CLEAN_VENDOR}"

    extract "${MY_DIR}/../${DEVICE}/proprietary-files.txt" "${SRC}" \
            "${KANG}" --section "${SECTION}"
fi

source "${MY_DIR}/setup-makefiles.sh"
