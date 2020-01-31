/*
 * Copyright (C) 2019 The LineageOS Project
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

#define LOG_TAG "TouchscreenGestureService"

#include <unordered_map>

#include <android-base/file.h>
#include <android-base/logging.h>

#include "TouchscreenGesture.h"

namespace {
typedef struct {
    int32_t keycode;
    const char* name;
    const char* path;
} GestureInfo;

// id -> info
const std::unordered_map<int32_t, GestureInfo> kGestureInfoMap = {
    {0, {254, "One finger up swipe", "/sys/devices/virtual/touchscreen/touchscreen_dev/gesture_ctrl"}},
    {1, {249, "One finger down swipe", "/sys/devices/virtual/touchscreen/touchscreen_dev/gesture_ctrl"}},
    {2, {250, "One finger left swipe", "/sys/devices/virtual/touchscreen/touchscreen_dev/gesture_ctrl"}},
    {3, {251, "One finger right swipe", "/sys/devices/virtual/touchscreen/touchscreen_dev/gesture_ctrl"}},
    {4, {252, "Letter C", "/sys/devices/virtual/touchscreen/touchscreen_dev/gesture_ctrl"}},
    {5, {255, "Letter E", "/sys/devices/virtual/touchscreen/touchscreen_dev/gesture_ctrl"}},
    {6, {256, "Letter M", "/sys/devices/virtual/touchscreen/touchscreen_dev/gesture_ctrl"}},
    {7, {253, "Letter O", "/sys/devices/virtual/touchscreen/touchscreen_dev/gesture_ctrl"}},
    {8, {257, "Letter W", "/sys/devices/virtual/touchscreen/touchscreen_dev/gesture_ctrl"}},
    {9, {260, "Letter S", "/sys/devices/virtual/touchscreen/touchscreen_dev/gesture_ctrl"}},
    {10, {259, "Letter V", "/sys/devices/virtual/touchscreen/touchscreen_dev/gesture_ctrl"}},
    {11, {258, "Letter Z", "/sys/devices/virtual/touchscreen/touchscreen_dev/gesture_ctrl"}},
};
}  // anonymous namespace

namespace vendor {
namespace lineage {
namespace touch {
namespace V1_0 {
namespace implementation {

Return<void> TouchscreenGesture::getSupportedGestures(getSupportedGestures_cb resultCb) {
    std::vector<Gesture> gestures;

    for (const auto& entry : kGestureInfoMap) {
        gestures.push_back({entry.first, entry.second.name, entry.second.keycode});
    }
    resultCb(gestures);

    return Void();
}

Return<bool> TouchscreenGesture::setGestureEnabled(
    const ::vendor::lineage::touch::V1_0::Gesture& gesture, bool enabled) {
    const auto entry = kGestureInfoMap.find(gesture.id);
    if (entry == kGestureInfoMap.end()) {
        return false;
    }

    if (!android::base::WriteStringToFile((enabled ? "1" : "0"), entry->second.path)) {
        LOG(ERROR) << "Wrote file " << entry->second.path << " failed";
        return false;
    }
    return true;
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace touch
}  // namespace lineage
}  // namespace vendor
