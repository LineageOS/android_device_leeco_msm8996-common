/*
 * Copyright (c) 2018 The LineageOS Project
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

package org.lineageos.settings.device;

import android.content.ContentResolver;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Build;
import android.os.SystemProperties;
import android.util.Log;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.lang.reflect.Field;
import java.lang.reflect.Method;

public class SettingsUtils {
    public static final String TAG = "SettingsUtils";
    public static final String CAMERA_FOCUS_FIX_ENABLED = "CAMERA_FOCUS_FIX_ENABLED";
    public static final String QUICK_CHARGE_ENABLED = "QUICK_CHARGE_ENABLED";

    public static final String CAMERA_FOCUS_FIX_SYSFS =
        "/sys/module/msm_actuator/parameters/use_focus_fix";
    public static final String QUICK_CHARGE_SYSFS =
        "/sys/class/power_supply/le_ab/le_quick_charge_mode";

    private static final String QC_SYSTEM_PROPERTY = "persist.sys.le_fast_chrg_enable";

    public static final String PREFERENCES = "SettingsUtilsPreferences";

    public static void writeCameraFocusFixSysfs(boolean enabled) {
        if (!supportsCameraFocusFix())
            return;
        try {
            FileOutputStream out = new FileOutputStream(new File(CAMERA_FOCUS_FIX_SYSFS));
            OutputStreamWriter writer = new OutputStreamWriter(out);

            writer.write(enabled ? '1' : '0');

            writer.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static void writeQuickChargeProp(boolean enabled) {
        SystemProperties.set(QC_SYSTEM_PROPERTY, enabled ? "1" : "0");
    }

    public static boolean supportsCameraFocusFix() {
        File focusFixPath = new File(CAMERA_FOCUS_FIX_SYSFS);
        return focusFixPath.exists();
    }

    public static boolean supportsQuickChargeSwitch() {
        File QCPath = new File(QUICK_CHARGE_SYSFS);
        return QCPath.exists();
    }

    public static boolean setCameraFocusFixEnabled(Context context, boolean enabled) {
        return putInt(context, CAMERA_FOCUS_FIX_ENABLED, enabled ? 1 : 0);
    }

    public static boolean getCameraFocusFixEnabled(Context context) {
        return getInt(context, CAMERA_FOCUS_FIX_ENABLED, 0) == 1;
    }

    public static boolean setQuickChargeEnabled(Context context, boolean enabled) {
        return putInt(context, QUICK_CHARGE_ENABLED, enabled ? 1 : 0);
    }

    public static boolean getQuickChargeEnabled(Context context) {
        return getInt(context, QUICK_CHARGE_ENABLED, 1) == 1;
    }

    public static int getInt(Context context, String name, int def) {
        SharedPreferences settings = context.getSharedPreferences(PREFERENCES, 0);
        return settings.getInt(name, def);
    }

    public static boolean putInt(Context context, String name, int value) {
        SharedPreferences settings = context.getSharedPreferences(PREFERENCES, 0);
        SharedPreferences.Editor editor = settings.edit();
        editor.putInt(name, value);
        return editor.commit();
    }
}
