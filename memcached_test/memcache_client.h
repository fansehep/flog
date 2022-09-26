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
 * Created Date: 2022-09-25
 * Author: YangFan (fansehep)
 */

#ifndef CURVEFS_SRC_CLIENT_S3_KVCLIENT_KVCLIENT_MEMCACHE_CLIENT_H_
#define CURVEFS_SRC_CLIENT_S3_KVCLIENT_KVCLIENT_MEMCACHE_CLIENT_H_

#include <libmemcached-1.0/memcached.h>
#include <libmemcachedutil-1.0/pool.h>

#include <map>
#include <mutex>
#include <thread>

#include "kvclient.h"

namespace curvefs {

namespace client {

/**
 * only the threadpool will operate the kvclient,
 * for threadsafe and fast, we can make every thread has a client.
 */
thread_local memcached_st* tcli = nullptr;

class MemCachedClient : public KvClient<MemCachedClient> {
 public:
    MemCachedClient() : server_(nullptr) {
        client_ = memcached_create(nullptr);
    }
    MemCachedClient(memcached_st* cli) : client_(cli) {}
    ~MemCachedClient() { UnInit(); }

 private:
    friend KvClient<MemCachedClient>;
    bool InitImp() {
        client_ = memcached(nullptr, 0);
        return client_ != nullptr;
    }

    void UnInitImp() {
        if (client_) {
            memcached_free(client_);
            client_ = nullptr;
        }
    }

    bool InitImp(const std::string& config) {
        client_ = memcached(config.c_str(), config.length());
        return client_ != nullptr;
    }

    bool SetImp(const std::string& key,
                const char* value,
                const int value_len,
                std::string* errorlog) {
        if (nullptr == tcli) {
            tcli = memcached_clone(tcli, client_);
        }
        auto res = memcached_set(tcli, key.c_str(), key.length(), value,
                                 value_len, 0, 0);
        if (MEMCACHED_SUCCESS == res) {
            return true;
        }
        *errorlog = ResError(res);
        return false;
    }

    bool GetImp(const std::string& key,
                std::string* value,
                std::string* errorlog) {
        if (nullptr == tcli) {
            tcli = memcached_clone(tcli, client_);
        }
        uint32_t flags = 0;
        size_t value_length = 0;
        memcached_return_t ue;
        char* res = memcached_get(tcli, key.c_str(), key.length(), &value_length,
                                  &flags, &ue);
        if (res != nullptr && value->empty()) {
            value->reserve(value_length + 1);
            value->assign(res, res + value_length + 1);
            value->resize(value_length);
            free(res);
            return true;
        }
        *errorlog = ResError(ue);
        return false;
    }

    // transform the res to a error string
    const std::string ResError(const memcached_return_t res) {
        return memcached_strerror(nullptr, res);
    }

 public:
    bool SetClientAttr(memcached_behavior_t flag, uint64_t data) {
        return (memcached_success(memcached_behavior_set(client_, flag, data)));
    }

    uint64_t GetClientAttr(memcached_behavior_t flag) {
        return memcached_behavior_get(client_, flag);
    }

    bool AddServer(const std::string& hostname,
                   const uint32_t port,
                   std::string* errorlog) {
        memcached_return_t res;
        server_ =
            memcached_server_list_append(server_, hostname.c_str(), port, &res);
        if (res == MEMCACHED_SUCCESS) {
            return true;
        }
        *errorlog = ResError(res);
        return false;
    }

    bool PushServer() {
        memcached_return_t res = memcached_server_push(client_, server_);
        memcached_server_list_free(server_);
        return res == MEMCACHED_SUCCESS;
    }

    int ServerCount() {
        return static_cast<int>(memcached_server_count(client_));
    }

 private:

    memcached_server_st* server_;
    memcached_st* client_;
};

} // namespace client

} // namespace curvefs

#endif