/*
   Copyright (C) 2007, The Android Open Source Project
   Copyright (c) 2016, The CyanogenMod Project
   Copyright (c) 2017-2019, The LineageOS Project

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/strings.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include "vendor_init.h"
#include "property_service.h"

#define DEVINFO_FILE "/dev/block/sde21"

using android::base::GetProperty;
using android::base::ReadFileToString;
using android::init::property_set;

void property_override(const std::string& name, const std::string& value)
{
    size_t valuelen = value.size();

    prop_info* pi = (prop_info*) __system_property_find(name.c_str());
    if (pi != nullptr) {
        __system_property_update(pi, value.c_str(), valuelen);
    }
    else {
        int rc = __system_property_add(name.c_str(), name.size(), value.c_str(), valuelen);
        if (rc < 0) {
            LOG(ERROR) << "property_set(\"" << name << "\", \"" << value << "\") failed: "
                       << "__system_property_add failed";
        }
    }
}

void property_overrride_triple(const std::string& product_prop, const std::string& system_prop, const std::string& vendor_prop, const std::string& value)
{
    property_override(product_prop, value);
    property_override(system_prop, value);
    property_override(vendor_prop, value);
}

void init_target_properties()
{
    std::string device;
    bool unknownDevice = true;

    if (ReadFileToString(DEVINFO_FILE, &device)) {
        LOG(INFO) << "DEVINFO: " << device;

        if (!strncmp(device.c_str(), "le_zl0_whole_netcom", 19)) {
            // This is LEX722
            property_overrride_triple("ro.product.device", "ro.product.system.device", "ro.vendor.product.device", "le_zl0");
            property_overrride_triple("ro.product.model", "ro.product.system.model", "ro.vendor.product.model", "LEX722");
            property_set("persist.data.iwlan.enable", "false");
            // Dual SIM
            property_set("persist.radio.multisim.config", "dsds");
            property_set("ro.telephony.default_network", "10,10");
            // Power profile
            property_set("ro.power_profile.override", "power_profile_zl0");
            unknownDevice = false;
        }
        else if (!strncmp(device.c_str(), "le_zl1_oversea", 14)) {
            // This is LEX727
            property_overrride_triple("ro.product.device", "ro.product.system.device", "ro.vendor.product.device", "le_zl1");
            property_overrride_triple("ro.product.model", "ro.product.system.model", "ro.vendor.product.model", "LEX727");
            property_overrride_triple("ro.product.name", "ro.product.system.name", "ro.vendor.product.name", "ZL1_NA");
            property_set("persist.data.iwlan.enable", "true");
            // Single SIM
            property_set("persist.radio.multisim.config", "NA");
            property_set("ro.telephony.default_network", "10");
            // NFC
            property_set("persist.nfc.smartcard.config", "SIM1,eSE1");
            unknownDevice = false;
        }
        else if (!strncmp(device.c_str(), "le_zl1", 6)) {
            // This is LEX720
            property_overrride_triple("ro.product.device", "ro.product.system.device", "ro.vendor.product.device", "le_zl1");
            property_overrride_triple("ro.product.model", "ro.product.system.model", "ro.vendor.product.model", "LEX720");
            property_set("persist.data.iwlan.enable", "false");
            // Dual SIM
            property_set("persist.radio.multisim.config", "dsds");
            property_set("ro.telephony.default_network", "10,10");
            // NFC
            property_set("persist.nfc.smartcard.config", "SIM1,SIM2,eSE1");
            unknownDevice = false;
        }
        else if (!strncmp(device.c_str(), "le_x2_na_oversea", 16)) {
            // This is LEX829
            property_overrride_triple("ro.product.device", "ro.product.system.device", "ro.vendor.product.device", "le_x2");
            property_overrride_triple("ro.product.model", "ro.product.system.model", "ro.product.vendor.model", "LEX829");
            property_overrride_triple("ro.product.name", "ro.product.system.name", "ro.vendor.product.name", "LeMax2_WW");
            // Dual SIM
            property_set("persist.radio.multisim.config", "dsds");
            property_set("ro.telephony.default_network", "10,10");
            unknownDevice = false;
        }
        else if (!strncmp(device.c_str(), "le_x2_india", 11)) {
            // This is LEX821
            property_overrride_triple("ro.product.device", "ro.product.system.device", "ro.vendor.product.device", "le_x2");
            property_overrride_triple("ro.product.model", "ro.product.system.model", "ro.vendor.product.model", "LEX821");
            property_overrride_triple("ro.product.name", "ro.product.system.name", "ro.vendor.product.name", "LeMax2_WW");
            // Dual SIM
            property_set("persist.radio.multisim.config", "dsds");
            property_set("ro.telephony.default_network", "10,10");
            unknownDevice = false;
        }
        else if (!strncmp(device.c_str(), "le_x2", 5)) {
            // This is LEX820
            property_overrride_triple("ro.product.device", "ro.product.system.device", "ro.vendor.product.device", "le_x2");
            property_overrride_triple("ro.product.model", "ro.product.system.model", "ro.vendor.product.model", "LEX820");
            property_overrride_triple("ro.product.name", "ro.product.system.name", "ro.vendor.product.name", "LeMax2_WW");
            // Dual SIM
            property_set("persist.radio.multisim.config", "dsds");
            property_set("ro.telephony.default_network", "10,10");
            unknownDevice = false;
        }
    }
    else {
        LOG(ERROR) << "Unable to read DEVINFO from " << DEVINFO_FILE;
    }

    if (unknownDevice) {
        property_override("ro.product.model", "UNKNOWN");
    }
}

void vendor_load_persist_properties() {
    LOG(INFO) << "Loading vendor specific properties";
    init_target_properties();
}
