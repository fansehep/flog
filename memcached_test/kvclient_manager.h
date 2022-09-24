/*
 *  Copyright (c) 2022 NetEase Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * Project: curve
 * Created Date: 2022-09-23
 * Author: YangFan (fansehep)
 */

#ifndef CURVEFS_SRC_CLIENT_S3_KVCLIENT_KVCLIENT_MANAGER_H_
#define CURVEFS_SRC_CLIENT_S3_KVCLIENT_KVCLIENT_MANAGER_H_


#include "kvclient.h"
#include <absl/synchronization/internal/thread_pool.h>
#include "kvclient_pool.h"

namespace curvefs {

namespace client {


using ::absl::synchronization_internal::ThreadPool;


class KvClientManager {
 public:
    KvClientManager() : threadPool_(1) {}
    bool Init(const std::string& kvclientconfigpath);
    bool Init(const std::vector<std::pair<std::string, uint32_t>>& conf, int inital, int max);
    bool Init();
    void Stop();
    void Set(const std::string& key, const char* value, const size_t value_len);
    bool Get(const std::string& key, std::string* value);
    void AddPoolClient(const std::vector<std::pair<std::string, uint32_t>>& conf);
    MemCachedPool GetPool();
 private:
    ThreadPool threadPool_;
    MemCachedPool cache_;
};

} // namespace client

} // namespace curvefs

#endif