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

package org.lineageos.consumerirtransmiter;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.IBinder;
import android.util.Log;

import java.util.ArrayList;
import java.util.List;

import android.content.ComponentName;
import android.content.ServiceConnection;

import org.lineageos.consumerirtransmiter.IControl;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;
import android.os.RemoteException;

public class ConsumerirTransmiterService extends Service {
    private static final String TAG = "ConsumerirTransmiter";

    private static final String ACTION_TRANSMIT_IR = "org.lineageos.consumerirtransmiter.TRANSMIT_IR";

    private final static String SYS_FILE_ENABLE_IR_BLASTER = "/sys/remote/enable";
    private boolean mBound = false;
    private IControl mControl;

    private BroadcastReceiver irReceiver;

    @Override
    public void onCreate() {
        Log.d(TAG, "Creating service");

        writeToSysFile("1");
        bindQuickSetService();

        irReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                String action = intent.getAction();
                if (ACTION_TRANSMIT_IR.equals(action)) {
                    if (intent.getStringExtra("carrier_freq") != null && intent.getStringExtra("pattern") != null) {
                        int carrierFrequency = Integer.parseInt(intent.getStringExtra("carrier_freq"));
                        String patternStr = intent.getStringExtra("pattern");
                        int[] pattern = Arrays.stream(patternStr.split(",")).map(String::trim)
                                .mapToInt(Integer::parseInt).toArray();

                        Log.d(TAG, "Received pattern at " + carrierFrequency + " hz");

                        transmitIrPattern(carrierFrequency, pattern);
                    }
                }
            }
        };

        registerReceiver(irReceiver, new IntentFilter(ACTION_TRANSMIT_IR));
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.d(TAG, "Starting service");

        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        Log.d(TAG, "Destroying service");
        super.onDestroy();

        this.unregisterReceiver(irReceiver);
        this.unbindService(mControlServiceConnection);
        writeToSysFile("0");
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    /**
     * Service Connection used to control the bound QuickSet SDK Service
     */
    private final ServiceConnection mControlServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            mBound = true;
            mControl = new IControl(service);
            Log.i(TAG, "QuickSet SDK Service (for controlling IR Blaster) SUCCESSFULLY CONNECTED! Yeah! :-)");
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            mBound = false;
            mControl = null;
            Log.i(TAG, "QuickSet SDK Service (for controlling IR Blaster) DISCONNECTED!");
        }
    };

    /**
     * Try to bind QuickSet SDK Service
     */
    public void bindQuickSetService() {
        Log.d(TAG, "Trying to bind QuickSet service (for controlling IR Blaster): " + IControl.QUICKSET_UEI_PACKAGE_NAME
                + " - " + IControl.QUICKSET_UEI_SERVICE_CLASS);
        try {
            Intent controlIntent = new Intent(IControl.ACTION);
            controlIntent.setClassName(IControl.QUICKSET_UEI_PACKAGE_NAME, IControl.QUICKSET_UEI_SERVICE_CLASS);
            boolean bindResult = bindService(controlIntent, mControlServiceConnection, Context.BIND_AUTO_CREATE);
            if (!bindResult) {
                Log.e(TAG,
                        "bindResult == false. QuickSet SDK service seems NOT TO BE AVAILABLE ON THIS PHONE! IR Blaster will probably NOT WORK!");
                Log.e(TAG, "QuickSet SDK service package/class: " + IControl.QUICKSET_UEI_PACKAGE_NAME + "/"
                        + IControl.QUICKSET_UEI_SERVICE_CLASS);
            } else {
                Log.d(TAG, "bindService() result: true");
            }
        } catch (Throwable t) {
            Log.e(TAG, "Binding QuickSet Control service failed with exception :-(", t);
        }
    }

    /**
     * Try to send Infrared pattern, catch and log exceptions.
     *
     * @param carrierFrequency carrier frequency, see ConsumerIrManager Android API
     * @param pattern          IR pattern to send, see ConsumerIrManager Android API
     */
    public int transmitIrPattern(int carrierFrequency, int[] pattern) {
        Log.d(TAG, "transmitIrPattern called: freq: " + carrierFrequency + ", pattern-len: " + pattern.length);
        if (mControl == null || !mBound) {
            Log.w(TAG, "QuickSet Service (for using IR Blaster) seems not to be bound. Trying to bind again and exit.");
            bindQuickSetService();
            // return something != 0 to indicate error
            return -1;
        }
        try {
            mControl.transmit(carrierFrequency, pattern);
            int resultCode = mControl.getLastResultcode();
            if (resultCode != 0) {
                Log.w(TAG,
                        "resultCode after calling transmit on QuickSet SDK was != 0. No idea what this means. lastResultcode: "
                                + resultCode);
            }
            return resultCode;
        } catch (Throwable t) {
            Log.e(TAG, "Exception while trying to send command to QuickSet Service. :-(", t);
            // return something != 0 to indicate error
            return -1;
        }
    }

    private static boolean writeToSysFile(String value) {
        File file = new File(SYS_FILE_ENABLE_IR_BLASTER);
        if (!file.exists() || !file.isFile() || !file.canWrite()) {
            return false;
        }
        try {
            FileWriter fileWriter = new FileWriter(file);
            fileWriter.write(value);
            fileWriter.flush();
            fileWriter.close();
        } catch (IOException e) {
            return false;
        }
        return true;
    }
}