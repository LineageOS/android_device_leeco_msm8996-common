/*
 * Copyright (C) 2013 The Android Open Source Project
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
#define LOG_TAG "ConsumerIrHal"

#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <string.h>
#include <cutils/log.h>
#include <linux/ioctl.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <hardware/hardware.h>
#include <hardware/consumerir.h>
#include <leeco_consumerir.h>

#include <termios.h>
#include <unistd.h>

static pthread_mutex_t g_mtx;

static int open_serial_device() {
    int fd_ir, speed;
    struct termios options;

    if (BAUD_RATE == 230400) {
        speed = B230400;
    } else if (BAUD_RATE == 115200) {
        speed = B115200;
    } else if (BAUD_RATE == 38400) {
        speed = B38400;
    } else {
        speed = B19200;
    }

    // Open serial device
    if ((fd_ir = open(IR_DEVICE, O_RDWR | O_NOCTTY | O_NDELAY)) == -1) {
        ALOGE("%s: Cannot open serial device ", __func__, IR_DEVICE);
        return(-1);
    }

    // Get current options for the serial port
    if (tcgetattr(fd_ir, &options)) {
        ALOGE("%s: tcgetattr() failed", __func__);
        return(-1);
    }

    // Init options
    cfmakeraw(&options);

    // Set input baud rate
    cfsetispeed(&options, speed);

    // Set output baud rate
    cfsetospeed(&options, speed);

    // Enable the receiver and set local mode
    if (tcsetattr(fd_ir, 0, &options)) {
        ALOGE("%s: tcsetattr() failed", __func__);
        return(-1);
    }

    return fd_ir;
}

static bool try_append_number(char *buffer, int *len, int size, int number)
{
    int stored;

    stored = snprintf(&buffer[*len], size - *len, "%d,", number);

    if (stored < 0 || stored >= (size - *len))
        return false;

    *len += stored;
    return true;
}

static bool grow_buffer(char **buffer, int *size)
{
    char *new_buffer;

    *size *= 2;
    new_buffer = realloc(*buffer, *size);
    if (new_buffer == NULL)
        return false;

    *buffer = new_buffer;
    return true;
}

static bool append_number(char **buffer, int *len, int *size, int number)
{
    if (try_append_number(*buffer, len, *size, number))
        return true;

    if (!grow_buffer(buffer, size))
        return false;

    return try_append_number(*buffer, len, *size, number);
}

static int consumerir_transmit(struct consumerir_device *dev __unused,
   int carrier_freq, const int pattern[], int pattern_len)
{
    int fd_enable, fd_ir = 0;
    int ret = -1;

    int buffer_len = 0;
    int buffer_size = 128;
    int i;
    float factor;
    char *buffer;

    pthread_mutex_lock(&g_mtx);

    ALOGE("%s", __func__);

    // Enable IR device
    fd_enable = open(IR_ENABLE_PATH, O_RDWR);
    write(fd_enable, "1", 1);

    // Open and configure serial device
    if ((fd_ir = open_serial_device()) == -1) {
        return 1;
    }

    buffer = malloc(buffer_size);
    if (buffer == NULL)
        goto error;

    // Write the header
    if (!append_number(&buffer, &buffer_len, &buffer_size, carrier_freq))
        goto error_buf;

    // Calculate factor of conversion from microseconds to pulses
    factor = 1000000 / carrier_freq;

    // Write out the timing pattern
    for (i = 0; i < pattern_len; i++) {
        if (!append_number(&buffer, &buffer_len, &buffer_size,
                (int) (pattern[i] / factor)))
            goto error_buf;
    }

    buffer[buffer_len - 1] = 0;

    // Write to serial device
    if (write(fd_ir, buffer, buffer_len - 1) < (buffer_len - 1)) {
        ALOGE("%s: failed writing", __func__);
    }

    free(buffer);

    // Close serial device
    close(fd_ir);

    // Disable IR device
    write(fd_enable, "0", 1);
    close(fd_enable);

    pthread_mutex_unlock(&g_mtx);

    return 0;

error_buf:
    free(buffer);
error:
    close(fd_ir);
    close(fd_enable);
    return 1;
}

static int consumerir_get_num_carrier_freqs(struct consumerir_device *dev __unused)
{
    ALOGE("%s", __func__);
    return ARRAY_SIZE(consumerir_freqs);
}

static int consumerir_get_carrier_freqs(struct consumerir_device *dev __unused,
    size_t len, consumerir_freq_range_t *ranges)
{
    ALOGE("%s", __func__);
    size_t to_copy = ARRAY_SIZE(consumerir_freqs);

    to_copy = len < to_copy ? len : to_copy;
    memcpy(ranges, consumerir_freqs, to_copy * sizeof(consumerir_freq_range_t));
    return to_copy;
}

static int consumerir_close(hw_device_t *dev)
{
    ALOGE("%s", __func__);
    free(dev);
    pthread_mutex_destroy(&g_mtx);
    return 0;
}

/*
 * Generic device handling
 */
static int consumerir_open(const hw_module_t* module, const char* name,
        hw_device_t** device)
{
    ALOGE("%s", __func__);

    if (strcmp(name, CONSUMERIR_TRANSMITTER) != 0) {
        return -EINVAL;
    }
    if (device == NULL) {
        ALOGE("NULL device on open");
        return -EINVAL;
    }

    consumerir_device_t *dev = malloc(sizeof(consumerir_device_t));
    memset(dev, 0, sizeof(consumerir_device_t));

    dev->common.tag = HARDWARE_DEVICE_TAG;
    dev->common.version = 0;
    dev->common.module = (struct hw_module_t*) module;
    dev->common.close = consumerir_close;

    dev->transmit = consumerir_transmit;
    dev->get_num_carrier_freqs = consumerir_get_num_carrier_freqs;
    dev->get_carrier_freqs = consumerir_get_carrier_freqs;

    pthread_mutex_init(&g_mtx, NULL);

    *device = (hw_device_t*) dev;
    return 0;
}

static struct hw_module_methods_t consumerir_module_methods = {
    .open = consumerir_open,
};

consumerir_module_t HAL_MODULE_INFO_SYM = {
    .common = {
        .tag                = HARDWARE_MODULE_TAG,
        .module_api_version = CONSUMERIR_MODULE_API_VERSION_1_0,
        .hal_api_version    = HARDWARE_HAL_API_VERSION,
        .id                 = CONSUMERIR_HARDWARE_MODULE_ID,
        .name               = "LeEco IR HAL",
        .author             = "The LineageOS Project",
        .methods            = &consumerir_module_methods,
    },
};
