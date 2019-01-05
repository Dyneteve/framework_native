/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <bufferhub/UniqueIdGenerator.h>

namespace android {
namespace frameworks {
namespace bufferhub {
namespace V1_0 {
namespace implementation {

constexpr uint32_t UniqueIdGenerator::kInvalidId;

uint32_t UniqueIdGenerator::getId() {
    std::lock_guard<std::mutex> lock(mIdsInUseMutex);

    do {
        if (++mLastId >= std::numeric_limits<uint32_t>::max()) {
            mLastId = kInvalidId + 1;
        }
    } while (mIdsInUse.find(mLastId) != mIdsInUse.end());

    mIdsInUse.insert(mLastId);
    return mLastId;
}

bool UniqueIdGenerator::freeId(uint32_t id) {
    std::lock_guard<std::mutex> lock(mIdsInUseMutex);
    auto iter = mIdsInUse.find(id);
    if (iter != mIdsInUse.end()) {
        mIdsInUse.erase(iter);
        return true;
    }

    return false;
}

} // namespace implementation
} // namespace V1_0
} // namespace bufferhub
} // namespace frameworks
} // namespace android
