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


#include <iostream>
#include <memory>
#include <glog/logging.h>
#include <absl/strings/string_view.h>
#include <absl/synchronization/internal/thread_pool.h>
#include <string_view>
#include "memcache_client.h"

namespace curvefs {

namespace client {

template <typename CacheClient>
struct KvClientManagerConfig {
    std::unique_ptr<CacheClient> kvclient;
    int threadPooln;
};

template <typename CacheClient>
class KvClientManager {
 public:
    KvClientManager() : threadPool_(1) {}
    ~KvClientManager() { Uninit(); }

    bool Init(KvClientManagerConfig<CacheClient>& config) {
        client_ = (std::move(config.kvclient));
        return true;
    }

    /**
     * close the connection with kv
     */
    void Uninit() {
        client_->UnInit();
    }

    /**
     * It will get a db client and set the key value asynchronusly.
     * The set task will push threadpool, you'd better
     * don't get the key immediately.
     */
    void Set(const std::string& key,
             const char* value,
             const size_t value_len) {
        threadPool_.Schedule([=]() {
            std::string error_log;
            auto res = client_->Set(key, value, value_len, &error_log);
            if (!res) {
                auto val_view = std::string_view(value, value_len);
                LOG(ERROR) << "Set key = " << key << " value = " << val_view
                           << " " << error_log;
            }
        });
    }

    /**
     * get value by key.
     * the value must be empty.
     */
    bool Get(const std::string& key, std::string* value) {
        std::string error_log;
        auto res = client_->Get(key, value, &error_log);
        if (!res) {
            VLOG(9) << "Get Key = " << key << " " << error_log;
        }
        return res;
    }

    CacheClient GetClient() {
        return client_.get();
    }

 private:
    absl::synchronization_internal::ThreadPool threadPool_;
    std::unique_ptr<CacheClient> client_;
};

} // namespace client

} // namespace curvefs

#endif