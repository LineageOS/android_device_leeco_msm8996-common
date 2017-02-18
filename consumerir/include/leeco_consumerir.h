/*
 * Copyright (C) 2017 The LineageOS Project
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

#ifndef LEECO_CONSUMERIR_H
#define LEECO_CONSUMERIR_H

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

/*
 * Board specific nodes
 *
 * If your kernel exposes these controls in another place, you can either
 * symlink to the locations given here, or override this header in your
 * device tree.
 */

#define IR_ENABLE_PATH "/sys/remote/enable"

#define IR_DEVICE "/dev/ttyHSL1"
#define BAUD_RATE 230400

/*
 * Board specific configs
 *
 * If your device needs a different configuration, you
 * can override this header in your device tree
 */

static const consumerir_freq_range_t consumerir_freqs[] = {
    {.min = 30000, .max = 30000},
    {.min = 33000, .max = 33000},
    {.min = 36000, .max = 36000},
    {.min = 38000, .max = 38000},
    {.min = 40000, .max = 40000},
    {.min = 56000, .max = 56000},
};

#define IOCTL_JPG_DECODE _IO(JPEG_IOCTL_MAGIC, 1)

#endif // LEECO_CONSUMERIR_H
