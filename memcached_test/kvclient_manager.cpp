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

#include "kvclient_manager.h"

#include <fcntl.h>
#include <openssl/conf.h>

#include "kvclient.h"

namespace curvefs {

namespace client {

bool KvClientManager::Init(const std::string& kvclientconfigpath) {
    auto fd = open(kvclientconfigpath.c_str(), O_RDONLY);
    if (fd < 0) {
        LOG(ERROR) << "init error config path = " << kvclientconfigpath;
        return false;
    }
    char buf[1024];
    read(fd, buf, sizeof(buf));
    close(fd);
    auto res = cache_.Init(buf, sizeof(buf));
    if (!res) {
        LOG(ERROR) << "init error config path = " << kvclientconfigpath;
        return false;
    }
    return true;
}

bool KvClientManager::Init(
    const std::vector<std::pair<std::string, uint32_t>>& conf,
    int inital,
    int max) {
    MemCachedClient client;
    for (auto& confiter : conf) {
        client.AddServer(confiter.first, confiter.second);
    }
    client.PushServer();
    auto res = cache_.Init(client, inital, max);
    return res;
}

void KvClientManager::Stop() {
    cache_.Stop();
}

void KvClientManager::Set(const std::string& key,
                          const char* value,
                          const size_t value_len) {
    threadPool_.Schedule([=]() {
        std::string error_log;
        auto res = cache_.GetKvClient().Set(key, value, value_len, &error_log);
        if (!res) {
            auto str_view = absl::string_view(value, value_len);
            LOG(ERROR) << "Set key = " << key << " value = " << str_view
                       << " error " << error_log;
        }
    });
}

bool KvClientManager::Get(const std::string& key, std::string* value) {
    auto client = cache_.GetKvClient();
    auto res = client.Get(key, value);
    if (!res) {
        LOG(INFO) << "Get key = " << key << " error " << client.ResError();
    }
    return res;
}

void KvClientManager::AddPoolClient(
    const std::vector<std::pair<std::string, uint32_t>>& conf) {
    MemCachedClient client;
    for (auto& confiter : conf) {
        client.AddServer(confiter.first, confiter.second);
    }
    client.PushServer();
    cache_.PutKvClient(client);
}

MemCachedPool KvClientManager::GetPool() {
    return cache_;
}

} // namespace client

} // namespace curvefs