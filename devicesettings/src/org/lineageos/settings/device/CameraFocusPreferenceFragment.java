/*
 * Copyright (C) 2018 The LineageOS Project
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

import android.os.Bundle;
import android.provider.Settings;
import androidx.preference.Preference;
import androidx.preference.PreferenceFragment;
import androidx.preference.SwitchPreference;

public class CameraFocusPreferenceFragment extends PreferenceFragment {
    private static final String KEY_CAMERA_FOCUS_FIX_ENABLE = "camera_focus_enable";

    private SwitchPreference mCameraFocusFixEnable;

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);
        getActivity().getActionBar().setDisplayHomeAsUpEnabled(true);
    }

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        addPreferencesFromResource(R.xml.leeco_settings_panel);

        mCameraFocusFixEnable = (SwitchPreference) findPreference(KEY_CAMERA_FOCUS_FIX_ENABLE);
        mCameraFocusFixEnable.setChecked(SettingsUtils.getCameraFocusFixEnabled(getActivity()));
        mCameraFocusFixEnable.setOnPreferenceChangeListener(mCameraFocusFixPrefListener);
    }

    @Override
    public void onResume() {
        super.onResume();
        getListView().setPadding(0, 0, 0, 0);
    }

    private Preference.OnPreferenceChangeListener mCameraFocusFixPrefListener =
        new Preference.OnPreferenceChangeListener() {
            @Override
            public boolean onPreferenceChange(Preference preference, Object value) {
                final String key = preference.getKey();

                if (KEY_CAMERA_FOCUS_FIX_ENABLE.equals(key)) {
                    boolean enabled = (boolean) value;
                    SettingsUtils.setCameraFocusFixEnabled(getActivity(), enabled);
                    SettingsUtils.writeCameraFocusFixSysfs(enabled);
                }

                return true;
            }
        };
}
