package org.lineageos.settings.device.utils;

/*
 * Copyright (C) 2020 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import android.content.Context;
import android.provider.Settings;
import android.text.TextUtils;
import android.util.Log;

public class SettingsProviderUtils {

    private static String TAG = "SettingProviderUtils";

    private static final boolean TETHERING_HW_ACCELERATION_ENABLE = false;
    private static final int TETHERING_HW_ACCELERATION_SETTING_VALUE_ON = 0;
    private static final int TETHERING_HW_ACCELERATION_SETTING_VALUE_OFF = 1;

    public static void overrideItems(Context context) {
        overrideTetheringHardwareAcceleration(context);
    }

    /*
     * override Tethering Hardware Acceleration in Developer options
     * enable Tethering Hardware Acceleration will make our leeco msm8996
     * ipv6 tethering won't work
     */
    private static void overrideTetheringHardwareAcceleration(Context context) {
        final String TETHERING_SETTING_GLOBAL_KEY =
                (String) ReflectionUtils.getStaticAttribute(
                        "android.provider.Settings$Global",
                        "TETHER_OFFLOAD_DISABLED");
        if (null == context
                || TextUtils.isEmpty(TETHERING_SETTING_GLOBAL_KEY)) {
            Log.e(TAG, "overrideTetheringHardwareAcceleration update failed");
            return;
        }
        final int hwTetheringOldValue = Settings.Global.getInt(
                context.getContentResolver(),
                TETHERING_SETTING_GLOBAL_KEY,
                -1);
        final boolean hwTetheringEnabledOldValue =
                hwTetheringOldValue == TETHERING_HW_ACCELERATION_SETTING_VALUE_ON;
        boolean needUpdate = hwTetheringOldValue < 0
                || (hwTetheringEnabledOldValue != TETHERING_HW_ACCELERATION_ENABLE);
        if (needUpdate) {
            try {
                Settings.Global.putInt(context.getContentResolver(),
                        TETHERING_SETTING_GLOBAL_KEY,
                        TETHERING_HW_ACCELERATION_ENABLE
                                ? TETHERING_HW_ACCELERATION_SETTING_VALUE_ON
                                : TETHERING_HW_ACCELERATION_SETTING_VALUE_OFF);
                Log.d(TAG, "overrideTetheringHardwareAcceleration --> "
                        + TETHERING_HW_ACCELERATION_ENABLE);
            } catch (Exception e) {
                e.printStackTrace();
                Log.e(TAG, "overrideTetheringHardwareAcceleration update failed, permission deny?");
            }

        } else {
            Log.d(TAG, "overrideTetheringHardwareAcceleration no need update");
        }
    }
}
