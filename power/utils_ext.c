/*
 * Copyright (c) 2016-2017, The Linux Foundation. All rights reserved.
 * Copyright (C) 2018 The LineageOS Project
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * *    * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#define LOG_NIDEBUG 0

#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "utils_ext.h"
#define LOG_TAG "QCOM PowerHAL-Ext"
#include <utils/Log.h>

#define PERF_HAL_PATH "libqti-perfd-client.so"
static void *qcopt_handle;
static int (*perf_lock_acq)(unsigned long handle, int duration,
    int list[], int numArgs);
static int (*perf_lock_rel)(unsigned long handle);
static int (*perf_hint)(int, char *, int, int);

static void *get_qcopt_handle()
{
    char qcopt_lib_path[PATH_MAX] = {0};
    void *handle = NULL;

    dlerror();

    if (property_get("ro.vendor.extension_library", qcopt_lib_path,
                NULL)) {
        handle = dlopen(qcopt_lib_path, RTLD_NOW);
        if (!handle) {
            ALOGE("Unable to open %s: %s\n", qcopt_lib_path,
                    dlerror());
        }
    }

    if (!handle) {
        handle = dlopen(PERF_HAL_PATH, RTLD_NOW);
        if (!handle) {
            ALOGE("Unable to open %s: %s\n", PERF_HAL_PATH,
                    dlerror());
        }
    }

    return handle;
}

static void __attribute__ ((constructor)) initialize(void)
{
    qcopt_handle = get_qcopt_handle();

    if (!qcopt_handle) {
        ALOGE("Failed to get qcopt handle.\n");
    } else {
        /*
         * qc-opt handle obtained. Get the perflock acquire/release
         * function pointers.
         */
        perf_lock_acq = dlsym(qcopt_handle, "perf_lock_acq");

        if (!perf_lock_acq) {
            ALOGE("Unable to get perf_lock_acq function handle.\n");
        }

        perf_lock_rel = dlsym(qcopt_handle, "perf_lock_rel");

        if (!perf_lock_rel) {
            ALOGE("Unable to get perf_lock_rel function handle.\n");
        }

        perf_hint = dlsym(qcopt_handle, "perf_hint");

        if (!perf_hint) {
            ALOGE("Unable to get perf_hint function handle.\n");
        }
    }
}

static void __attribute__ ((destructor)) cleanup(void)
{
    if (qcopt_handle) {
        if (dlclose(qcopt_handle))
            ALOGE("Error occurred while closing qc-opt library.");
    }
}


//Same as perf_hint_enable, but with the ability to
//choose the type
int perf_hint_enable_with_type(int hint_id, int duration, int type)
{
    int lock_handle = 0;

    if (qcopt_handle) {
        if (perf_hint) {
            lock_handle = perf_hint(hint_id, NULL, duration, type);
            if (lock_handle == -1)
                ALOGE("Failed to acquire lock.");
        }
    }
    return lock_handle;
}

