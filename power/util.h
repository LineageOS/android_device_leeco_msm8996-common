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
#ifndef POWER_UTIL_H
#define POWER_UTIL_H

void sysfs_write(const char *path, const char *s);
int sysfs_write_silent(const char *path, const char *s);
int sysfs_read(const char *path, char *s, int num_bytes);
int sysfs_read_buf(const char* path, char *buf, int size);
int get_max_freq(int cpu, char *freq, int size);
int get_max_cpus(char *cpus, int size);
int write_cpufreq_value(int cpu, const char* key, const char* value);
int set_online_state(int cpu, const char* value);
#endif
