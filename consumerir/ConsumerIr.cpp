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

#define LOG_TAG "ConsumerIrService"

#include <fcntl.h>

#include <android-base/logging.h>

#include <iostream>
#include <vector>
#include "ConsumerIr.h"

#include <string>
#include "UDPClient.h"

namespace android {
namespace hardware {
namespace ir {
namespace V1_0 {
namespace implementation {

static hidl_vec<ConsumerIrFreqRange> rangeVec{
    {.min = 30000, .max = 30000},
    {.min = 33000, .max = 33000},
    {.min = 36000, .max = 36000},
    {.min = 38000, .max = 38000},
    {.min = 40000, .max = 40000},
    {.min = 56000, .max = 56000},
};

// Methods from ::android::hardware::ir::V1_0::IConsumerIr follow.
Return<bool> ConsumerIr::transmit(int32_t carrierFreq, const hidl_vec<int32_t>& pattern) {
    if (pattern.size() > 0) {
        std::ostringstream vts;

        std::copy(pattern.begin(), pattern.end(), std::ostream_iterator<int32_t>(vts, ","));

        vts << carrierFreq;

        std::string msg = vts.str();
        UDPClient *client = new UDPClient("localhost", 8877);
        client->send(msg.c_str(), msg.size());

        return true;
    }

    return false;
}

Return<void> ConsumerIr::getCarrierFreqs(getCarrierFreqs_cb _hidl_cb) {
    _hidl_cb(true, rangeVec);
    return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace ir
}  // namespace hardware
}  // namespace android
