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
 * Created Date: 2022-09-22
 * Author: YangFan (fansehep)
 */
#ifndef CURVEFS_SRC_CLIENT_S3_KVCLIENT_KVCLIENT_H_
#define CURVEFS_SRC_CLIENT_S3_KVCLIENT_KVCLIENT_H_

#include <libmemcached-1.0/memcached.h>
#include <libmemcached-1.0/return.h>
#include <libmemcached-1.0/strerror.h>
#include <libmemcached-1.0/types/return.h>
#include <libmemcached/util.h>
#include <libmemcachedutil-1.0/pool.h>
#include <netinet/in.h>

#include <atomic>
#include <chrono>
#include <libmemcached-1.0/memcached.hpp>
#include <thread>

namespace curvefs {

namespace client {

template <typename T>
class KvClient {
 private:
    T* Imp() { return static_cast<T*>(this); }

 public:
    KvClient() = default;
    bool Init() { return Imp()->InitImp(); }
    bool Init(const std::string& config) { return Imp()->InitImp(config); }
    bool Init(const std::string& hostname, uint32_t port) {
        return Imp()->InitImp(hostname, port);
    }
    bool Set(const std::string& key,
             const char* value,
             const int value_len,
             std::string* errorlog) {
        return Imp()->SetImp(key, value, value_len, errorlog);
    }

    bool Get(const std::string& key, std::string* value) {
        return Imp()->GetImp(key, value);
    }

    bool AddServer(const std::string& hostname, const uint32_t port) {
        return Imp()->AddServerImp(hostname, port);
    }
    bool PushServer() { return Imp()->PushServerImp(); }
};

class MemCachedClient : public KvClient<MemCachedClient> {
 public:
    MemCachedClient() : server_(nullptr) {
        client_ = memcached_create(nullptr);
    }

    MemCachedClient(memcached_st* cli) : client_(cli) {}

    bool InitImp() {
        client_ = memcached(nullptr, 0);
        return client_ != nullptr;
    }

    bool InitImp(const std::string& config) {
        client_ = memcached(config.c_str(), config.length());
        return client_ != nullptr;
    }

    bool InitImp(const std::string& hostname, uint32_t port) {
        client_ = memcached(nullptr, 0);
        if (client_) {
            res_ = memcached_server_add(client_, hostname.c_str(), port);
            return res_ == MEMCACHED_SUCCESS;
        }
        return false;
    }

    bool SetImp(const std::string& key,
                const char* value,
                const int value_len,
                std::string* errorlog) {
        auto res = memcached_set(client_, key.c_str(), key.length(), value,
                                 value_len, 0, 0);
        if (res == MEMCACHED_SUCCESS) {
            return true;
        }
        *errorlog = ResError(res);
        return false;
    }

    bool GetImp(const std::string& key, std::string* value) {
        uint32_t flags = 0;
        size_t value_length = 0;
        char* res = memcached_get(client_, key.c_str(), key.length(),
                                  &value_length, &flags, &res_);
        if (res != nullptr && value->empty()) {
            value->reserve(value_length + 1);
            value->assign(res, res + value_length + 1);
            value->resize(value_length);
            free(res);
            return true;
        }
        return false;
    }

    bool AddServerImp(const std::string& hostname, const uint32_t port) {
        server_ = memcached_server_list_append(server_, hostname.c_str(), port,
                                               &res_);
        return res_ == MEMCACHED_SUCCESS;
    }

    bool PushServerImp() {
        res_ = memcached_server_push(client_, server_);
        return res_ == MEMCACHED_SUCCESS;
    }

    bool SetClientAttr(memcached_behavior_t flag, uint64_t data) {
        return (memcached_success(memcached_behavior_set(client_, flag, data)));
    }

    uint64_t GetClientAttr(memcached_behavior_t flag) {
        return memcached_behavior_get(client_, flag);
    }

    const std::string ResError() const {
        return memcached_strerror(nullptr, res_);
    }

    const std::string ResError(const memcached_return_t res) {
        return memcached_strerror(nullptr, res);
    }

    memcached_return_t res_;
    memcached_server_st* server_;
    memcached_st* client_;
};

} // namespace client

} // namespace curvefs

#endif