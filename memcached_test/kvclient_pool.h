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

#include <assert.h>
#include <glog/logging.h>
#include <libmemcached-1.0/types/return.h>
#include <libmemcachedutil-1.0/pool.h>

#include "kvclient.h"

namespace curvefs {

namespace client {

template <typename kvclient, typename pool>
class KvClientPool {
 private:
    pool* Imp() { return static_cast<pool*>(this); }

 public:
    KvClientPool() = default;

    kvclient GetKvClient() { return Imp()->GetKvClientImp(); }

    bool Init(const char* buf, const size_t buf_len) {
        return Imp()->InitImp(buf, buf_len);
    }

    bool Init(const MemCachedClient& client, int inital, int max) {
        return Imp()->InitImp(client, inital, max);
    }

    void Stop() { return Imp()->StopImp(); }

    bool AddServer(const std::string& hostname, const uint32_t port) {
        return Imp()->AddServerImp(hostname, port);
    }
};

class MemCachedPool : public KvClientPool<MemCachedClient, MemCachedPool> {
 public:

    MemCachedPool() : clientcount_(0), pool_(nullptr) {}

    bool InitImp(const MemCachedClient& client, int inital, int max) {
        assert(client.client_ != nullptr);
        pool_ =
            memcached_pool_create(client.client_, inital, max > 32 ? max : 32);
        return pool_ != nullptr;
    }

    bool InitImp(const char* buf, const size_t buf_len) {
        pool_ = memcached_pool(buf, buf_len);
        return pool_ != nullptr;
    }

    void StopImp() {
        if (!pool_) {
            return;
        }
        memcached_pool_destroy(pool_);
    }

    MemCachedClient GetKvClientImp() {
        struct timespec tim = {0, 0};
        memcached_return_t res;
        auto cli = memcached_pool_fetch(pool_, &tim, &res);
        if (cli) {
            return MemCachedClient(cli);
        } else {
            LOG(WARNING) << "No MemCacheClient left";
            return {};
        }
    }

    bool PutKvClient(const MemCachedClient& memclient) {
        clientcount_++;
        assert(nullptr != pool_);
        memcached_return_t res;
        res = memcached_pool_push(pool_, memclient.client_);
        return res == MEMCACHED_SUCCESS;
    }

    memcached_return_t SetBehavior(const memcached_behavior_t flag,
                                   const uint64_t data) {
        return memcached_pool_behavior_set(pool_, flag, data);
    }

    memcached_return_t GetBehavior(const memcached_behavior_t flag,
                                   uint64_t* data) {
        return memcached_pool_behavior_get(pool_, flag, data);
    }

 private:
    int clientcount_;
    memcached_pool_st* pool_;
};

} // namespace client

} // namespace curvefs