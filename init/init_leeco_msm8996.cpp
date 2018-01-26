/*
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

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include <android-base/strings.h>

#include "vendor_init.h"
#include "property_service.h"
#include "log.h"
#include "util.h"

using android::base::Trim;

#define DEVINFO_FILE "/dev/block/bootdevice/by-name/devinfo"

void property_override(char const prop[], char const value[])
{
    prop_info *pi;

    pi = (prop_info*) __system_property_find(prop);
    if (pi)
        __system_property_update(pi, value, strlen(value));
    else
        __system_property_add(prop, strlen(prop), value, strlen(value));
}

static int read_file2(const char *fname, char *data, int max_size)
{
    int fd, rc;

    if (max_size < 1)
        return 0;

    fd = open(fname, O_RDONLY);
    if (fd < 0) {
        ERROR("failed to open '%s'\n", fname);
        return 0;
    }

    rc = read(fd, data, max_size - 1);
    if ((rc > 0) && (rc < max_size))
        data[rc] = '\0';
    else
        data[0] = '\0';
    close(fd);

    return 1;
}

void init_alarm_boot_properties()
{
    char const *boot_reason_file = "/proc/sys/kernel/boot_reason";
    char const *power_off_alarm_file = "/persist/alarm/powerOffAlarmSet";
    std::string boot_reason;
    std::string power_off_alarm;
    std::string reboot_reason = property_get("ro.boot.alarmboot");

    if (read_file(boot_reason_file, &boot_reason)
            && read_file(power_off_alarm_file, &power_off_alarm)) {        /*
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
        if (Trim(boot_reason) =="0") {
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
        else if ((Trim(boot_reason) == "3"  || reboot_reason == "true")
                  && Trim(power_off_alarm) == "1") {
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
}

void vendor_load_properties() {
    char device[PROP_VALUE_MAX];
    int isLEX720 = 0, isLEX722 = 0, isLEX727 = 0, isLEX820 = 0, isLEX829 = 0;

    if (read_file2(DEVINFO_FILE, device, sizeof(device)))
    {
        if (!strncmp(device, "le_zl0_whole_netcom", 19))
        {
            isLEX722 = 1;
        }
        else if (!strncmp(device, "le_zl1_oversea", 14))
        {
            isLEX727 = 1;
        }
        else if (!strncmp(device, "le_zl1", 6))
        {
            isLEX720 = 1;
        }
        else if (!strncmp(device, "le_x2_na_oversea", 16))
        {
            isLEX829 = 1;
        }
        else if (!strncmp(device, "le_x2", 5))
        {
            isLEX820 = 1;
        }
    }

    if (isLEX720)
    {
        // This is LEX720
        property_override("ro.product.model", "LEX720");
        property_override("ro.product.name", "ZL1_CN");
        property_set("persist.data.iwlan.enable", "false");
        // Dual SIM
        property_set("persist.radio.multisim.config", "dsds");
    }
    else if (isLEX722)
    {
        // This is LEX722
        property_override("ro.product.device", "le_zl0");
        property_override("ro.product.model", "LEX722");
        property_override("ro.product.name", "ZL1_CN");
        property_set("persist.data.iwlan.enable", "false");
        // Dual SIM
        property_set("persist.radio.multisim.config", "dsds");
        // Power profile
        property_set("ro.power_profile.override", "power_profile_zl0");
        // Fingerprint
        property_override("ro.build.description", "le_zl0-user 6.0.1 WIXCNFN5802001232S eng.letv.20170123.152935 release-keys");
        property_override("ro.build.fingerprint", "LeEco/ZL1_CN/le_zl0:6.0.1/WIXCNFN5802001232S/letv01231534:user/release-keys");
  
    }
    else if (isLEX727)
    {
        // This is LEX727
        property_override("ro.product.model", "LEX727");
        property_override("ro.product.name", "ZL1_NA");
        property_set("persist.data.iwlan.enable", "true");
        // Single SIM
        property_set("persist.radio.multisim.config", "NA");
    }
    else if (isLEX820)
    {
        // This is LEX820
        property_override("ro.product.model", "LEX820");
        // Dual SIM
        property_set("persist.radio.multisim.config", "dsds");
    }
    else if (isLEX829)
    {
        // This is LEX829
        property_override("ro.product.model", "LEX829");
        // Dual SIM
        property_set("persist.radio.multisim.config", "dsds");
    }
    else
    {
        property_override("ro.product.model", "UNKNOWN");
    }

    init_alarm_boot_properties();
}
