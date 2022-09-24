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

#include <absl/synchronization/internal/thread_pool.h>

#include "kvclient.h"
#include "kvclient_pool.h"

namespace curvefs {

namespace client {

using ::absl::synchronization_internal::ThreadPool;

class KvClientManager {
 public:
    KvClientManager() : threadPool_(1) {}
    ~KvClientManager();
    /**
     * config with the file path
     * @param: kvclientconfigpath: the config file path
     * @return: init success return true, otherwise return false
     */
    bool Init(const std::string& kvclientconfigpath);

    /**
     * config with the host + port
     * @param: conf: a nums of {ip, port}
     * @param: inital: the default number of kv connection pool connections
     * @param: max: the connection max number of connections. default is 32
     * @return: init success return true, otherwise return false
     */
    bool Init(const std::vector<std::pair<std::string, uint32_t>>& conf,
              int inital,
              int max);

    /**
     * close the connection with kv
     */
    void Stop();

    /**
     * It will get a db client and set the key value asynchronusly.
     * The set task will push threadpool, you'd better
     * don't get the key immediately.
     */
    void Set(const std::string& key, const char* value, const size_t value_len);

    /**
     * get value by key.
     * the value must be empty.
     */
    bool Get(const std::string& key, std::string* value);

    /**
     * add a client to connection pool with the conf
     */
    void AddPoolClient(
        const std::vector<std::pair<std::string, uint32_t>>& conf);
    MemCachedPool GetPool();

 private:
    ThreadPool threadPool_;
    MemCachedPool cache_;
};

} // namespace client

} // namespace curvefs

#endif