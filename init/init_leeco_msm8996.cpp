/*
   Copyright (C) 2007, The Android Open Source Project
   Copyright (c) 2016, The CyanogenMod Project
   Copyright (c) 2017, The LineageOS Project

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
#include <stdio.h>
#include <stdlib.h>
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
#include "util.h"

#define DEVINFO_FILE "/dev/block/sde21"

using android::base::Trim;
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

void init_target_properties()
{
    std::string device;
    int unknownDevice = 1;

    if (ReadFileToString(DEVINFO_FILE, &device)) {
        LOG(INFO) << "DEVINFO: " << device;

        if (!strncmp(device.c_str(), "le_zl0_whole_netcom", 19)) {
            // This is LEX722
            property_override("ro.product.device", "le_zl0");
            property_override("ro.product.model", "LEX722");
            property_override("ro.product.name", "LePro3 Elite");
            property_set("persist.data.iwlan.enable", "false");
            // Dual SIM
            property_set("persist.radio.multisim.config", "dsds");
            unknownDevice = 0;
        }
        else if (!strncmp(device.c_str(), "le_zl1_oversea", 14)) {
            // This is LEX727
            property_override("ro.product.model", "LEX727");
            property_set("persist.data.iwlan.enable", "true");
            // Single SIM
            property_set("persist.radio.multisim.config", "NA");
            unknownDevice = 0;
        }
        else if (!strncmp(device.c_str(), "le_zl1", 6)) {
            // This is LEX720
            property_override("ro.product.model", "LEX720");
            property_set("persist.data.iwlan.enable", "false");
            // Dual SIM
            property_set("persist.radio.multisim.config", "dsds");
            unknownDevice = 0;
        }
        else if (!strncmp(device.c_str(), "le_x2_na_oversea", 16)) {
            // This is LEX829
            property_override("ro.product.model", "LEX829");
            // Dual SIM
            property_set("persist.radio.multisim.config", "dsds");
            unknownDevice = 0;
        }
        else if (!strncmp(device.c_str(), "le_x2", 5)) {
            // This is LEX820
            property_override("ro.product.model", "LEX820");
            // Dual SIM
            property_set("persist.radio.multisim.config", "dsds");
            unknownDevice = 0;
        }
    }
    else {
        LOG(ERROR) << "Unable to read DEVINFO from " << DEVINFO_FILE;
    }

    if (unknownDevice) {
        property_override("ro.product.model", "UNKNOWN");
    }
}

void init_alarm_boot_properties()
{
    char const *boot_reason_file = "/proc/sys/kernel/boot_reason";
    std::string boot_reason;

    if (ReadFileToString(boot_reason_file, &boot_reason)) {
        /*
         * Setup ro.alarm_boot value to true when it is RTC triggered boot up
         * For existing PMIC chips, the following mapping applies
         * for the value of boot_reason:
         *
         * 0 -> unknown
         * 1 -> hard reset
         * 2 -> sudden momentary power loss (SMPL)
         * 3 -> real time clock (RTC)
         * 4 -> DC charger inserted
         * 5 -> USB charger insertd
         * 6 -> PON1 pin toggled (for secondary PMICs)
         * 7 -> CBLPWR_N pin toggled (for external power supply)
         * 8 -> KPDPWR_N pin toggled (power key pressed)
         */
        if (Trim(boot_reason) == "0") {
            property_set("ro.boot.bootreason", "invalid");
            property_set("ro.alarm_boot", "false");
        }
        else if (Trim(boot_reason) == "1") {
            property_set("ro.boot.bootreason", "hard_reset");
            property_set("ro.alarm_boot", "false");
        }
        else if (Trim(boot_reason) == "2") {
            property_set("ro.boot.bootreason", "smpl");
            property_set("ro.alarm_boot", "false");
        }
        else if (Trim(boot_reason) == "3") {
            property_set("ro.alarm_boot", "true");
        }
        else if (Trim(boot_reason) == "4") {
            property_set("ro.boot.bootreason", "dc_chg");
            property_set("ro.alarm_boot", "false");
        }
        else if (Trim(boot_reason) == "5") {
            property_set("ro.boot.bootreason", "usb_chg");
            property_set("ro.alarm_boot", "false");
        }
        else if (Trim(boot_reason) == "6") {
            property_set("ro.boot.bootreason", "pon1");
            property_set("ro.alarm_boot", "false");
        }
        else if (Trim(boot_reason) == "7") {
            property_set("ro.boot.bootreason", "cblpwr");
            property_set("ro.alarm_boot", "false");
        }
        else if (Trim(boot_reason) == "8") {
            property_set("ro.boot.bootreason", "kpdpwr");
            property_set("ro.alarm_boot", "false");
        }
    }
    else {
        LOG(ERROR) << "Unable to read bootreason from " << boot_reason_file;
    }
}

void vendor_load_properties() {
    LOG(INFO) << "Loading vendor specific properties";
    init_target_properties();
    init_alarm_boot_properties();
}
