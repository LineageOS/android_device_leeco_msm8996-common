# Copyright (C) 2016 The CyanogenMod Project
# Copyright (C) 2017 The The LineageOS Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Inherit from those products. Most specific first.
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

# Inherit from oneplus3 device
$(call inherit-product, device/leeco/zl1/device.mk)

# Inherit some common LineageOS stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

PRODUCT_NAME := lineage_zl1
PRODUCT_DEVICE := zl1
PRODUCT_MANUFACTURER := LeEco
PRODUCT_BRAND := LeEco

PRODUCT_GMS_CLIENTID_BASE := android-leeco

TARGET_VENDOR_PRODUCT_NAME := LePro3
TARGET_VENDOR_DEVICE_NAME := le_zl1
PRODUCT_BUILD_PROP_OVERRIDES += TARGET_DEVICE=le_zl1 PRODUCT_NAME=LePro3

PRODUCT_BUILD_PROP_OVERRIDES += \
    BUILD_FINGERPRINT=LeEco/le_zl1/LePro3:6.0.1/MMB29M/362280:user/release-keys \
    PRIVATE_BUILD_DESC="LePro3-user 6.0.1 MMB29M 24 dev-keys"

PRODUCT_SYSTEM_PROPERTY_BLACKLIST += ro.product.model

TARGET_VENDOR := leeco
