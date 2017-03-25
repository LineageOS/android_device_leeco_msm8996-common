/*
 * Copyright (C) 2012 The Android Open Source Project
 * Copyright (C) 2014 The OmniROM Project
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
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <dlfcn.h>

#define LOG_TAG "PowerHAL"
#include <utils/Log.h>

//#define LOG_NDEBUG 0

#define CPUFREQ_ROOT_PATH "/sys/devices/system/cpu/cpu"
#define CPUFREQ_TAIL_PATH "/cpufreq/"

// what is the max frequency (for using max value)
#define MAX_FREQ_PATH "cpuinfo_max_freq"
// how much cpus ara available (for using max value) e.g. 0-3
#define CPUS_MAX_PATH "/sys/devices/system/cpu/present"
#define ONLINE_PATH "/online"

#define PROFILE_MAX_TAG "max"

void sysfs_write(const char *path, const char *s)
{
    char buf[80];
    int len;
    int fd = open(path, O_WRONLY);

    if (fd < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error opening %s: %s\n", path, buf);
        return;
    }

    len = write(fd, s, strlen(s));
    if (len < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error writing to %s: %s\n", path, buf);
    }

    close(fd);
}

int sysfs_write_silent(const char *path, const char *s)
{
    int len;
    int rc = 0;
    int fd = open(path, O_WRONLY);

    if (fd < 0) {
        return -1;
    }

    len = write(fd, s, strlen(s));
    if (len < 0) {
        rc = -1;
    }
    close(fd);
    return rc;
}

int sysfs_read(const char *path, char *s, int num_bytes)
{
    char buf[80];
    int count;
    int ret = 0;
    int fd = open(path, O_RDONLY);

    if (fd < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error opening %s: %s\n", path, buf);

        return -1;
    }

    if ((count = read(fd, s, num_bytes - 1)) < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error reading %s: %s\n", path, buf);

        ret = -1;
    } else {
        s[count] = '\0';
    }

    close(fd);

    return ret;
}

int sysfs_read_buf(const char* path, char *buf, int size) {
    if (sysfs_read(path, buf, size) == -1) {
        return -1;
    } else {
        // Strip newline at the end.
        int len = strlen(buf);

        len--;

        while (len >= 0 && (buf[len] == '\n' || buf[len] == '\r'))
            buf[len--] = '\0';
    }

    return 0;
}

int get_max_freq(int cpu, char *freq, int size) {
    char cpufreq_path[256];

    sprintf(cpufreq_path, "%s%d%s%s", CPUFREQ_ROOT_PATH, cpu, CPUFREQ_TAIL_PATH, MAX_FREQ_PATH);
    return sysfs_read_buf(cpufreq_path, freq, size);
}

int set_online_state(int cpu, const char* value) {
    char online_path[256];

    sprintf(online_path, "%s%d%s", CPUFREQ_ROOT_PATH, cpu, ONLINE_PATH);
    int rc = sysfs_write_silent(online_path, value);
    if (!rc) {
        ALOGI("write %s -> %s", online_path, value);
    }
    return rc;
}

int get_max_cpus(char *cpus, int size) {
    return sysfs_read_buf(CPUS_MAX_PATH, cpus, size);
}

int write_cpufreq_value(int cpu, const char* key, const char* value) {
    char cpufreq_path[256];

    sprintf(cpufreq_path, "%s%d%s%s", CPUFREQ_ROOT_PATH, cpu, CPUFREQ_TAIL_PATH, key);
    int rc = sysfs_write_silent(cpufreq_path, value);
    if (!rc) {
        ALOGI("write %s -> %s", cpufreq_path, value);
    }
    return rc;
}
