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
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;

import java.lang.reflect.Field;
import java.lang.reflect.Method;

public class SettingsUtils {
    public static final String TAG = "SettingsUtils";
    public static final String CAMERA_FOCUS_FIX_ENABLED =
            "CAMERA_FOCUS_FIX_ENABLED";

    public static final String CAMERA_FOCUS_FIX_SYSFS =
            "/sys/module/msm_actuator/parameters/use_focus_fix";

    public static final String PREFERENCES = "SettingsUtilsPreferences";
    public static final String SETTINGS_CLASS = "lineageos.providers.LineageSettings$System";

    public static void writeCameraFocusFixSysfs(boolean enabled) {
        try {
            FileOutputStream out = new FileOutputStream(new File(CAMERA_FOCUS_FIX_SYSFS));
            OutputStreamWriter writer = new OutputStreamWriter(out);

            writer.write(enabled ? '1' : '0');

            writer.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static boolean setCameraFocusFixEnabled(Context context, boolean enabled) {
        return putIntSystem(context, context.getContentResolver(), CAMERA_FOCUS_FIX_ENABLED, enabled ? 1 : 0);
    }

    public static boolean getCameraFocusFixEnabled(Context context) {
        return getIntSystem(context, context.getContentResolver(), CAMERA_FOCUS_FIX_ENABLED, 0) == 1;
    }

    public static int getIntSystem(Context context, ContentResolver cr, String name, int def) {
        int ret;

        try {
            Class systemSettings = Class.forName(SETTINGS_CLASS);
            Method getInt = systemSettings.getMethod("getInt", ContentResolver.class,
                    String.class, int.class);
            String sdkName = (String)systemSettings.getDeclaredField(name).get(null);
            ret = (int)getInt.invoke(systemSettings, cr, sdkName, def);
        } catch (Exception e) {
            Log.i(TAG, "CMSettings not found. Using application settings for getInt");
            ret = getInt(context, name, def);
        }

        return ret;
    }

    public static int getInt(Context context, String name, int def) {
        SharedPreferences settings = context.getSharedPreferences(PREFERENCES, 0);
        return settings.getInt(name, def);
    }

    public static boolean putIntSystem(Context context, ContentResolver cr, String name, int value) {
        boolean ret;

        try {
            Class systemSettings = Class.forName(SETTINGS_CLASS);
            Method putInt = systemSettings.getMethod("putInt", ContentResolver.class,
                    String.class, int.class);
            String sdkName = (String)systemSettings.getDeclaredField(name).get(null);
            ret = (boolean)putInt.invoke(systemSettings, cr, sdkName, value);
        } catch (Exception e) {
            Log.i(TAG, "CMSettings not found. Using application settings for putInt");
            ret = putInt(context, name, value);
        }

        return ret;
    }

    public static boolean putInt(Context context, String name, int value) {
        SharedPreferences settings = context.getSharedPreferences(PREFERENCES, 0);
        SharedPreferences.Editor editor = settings.edit();
        editor.putInt(name, value);
        return editor.commit();
    }

    public static void registerPreferenceChangeListener(Context context,
            SharedPreferences.OnSharedPreferenceChangeListener preferenceListener) {
        SharedPreferences settings = context.getSharedPreferences(PREFERENCES, 0);
        settings.registerOnSharedPreferenceChangeListener(preferenceListener);
    }

    public static void unregisterPreferenceChangeListener(Context context,
            SharedPreferences.OnSharedPreferenceChangeListener preferenceListener) {
        SharedPreferences settings = context.getSharedPreferences(PREFERENCES, 0);
        settings.unregisterOnSharedPreferenceChangeListener(preferenceListener);
    }

}
