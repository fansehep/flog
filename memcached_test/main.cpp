#include <libmemcached-1.0/types/behavior.h>
#include <string.h>
#include <uuid/uuid.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "kvclient_manager.h"
#include "memcache_client.h"

using ::curvefs::client::KvClientManager;
using ::curvefs::client::KvClientManagerConfig;
using ::curvefs::client::MemCachedClient;

// simple_test pass
int main() {
    KvClientManager<MemCachedClient> client;
    std::unique_ptr<MemCachedClient> client_ =
        std::make_unique<MemCachedClient>();
    std::string res;
    auto ue = client_->AddServer("127.0.0.1", 18080);
    if (!ue) {
        LOG(ERROR) << "addserver error " << res;
    }
    ue = client_->PushServer();
    if (!ue) {
        LOG(ERROR) << "push server error";
    }
    uint64_t data;
    MEMCACHED_BEHAVIOR_TCP_NODELAY, data);client_->SetClientAttr(MEMCACHED_BEHAVIOR_NO_BLOCK, data);
    client_->SetClientAttr(
    auto conf = KvClientManagerConfig<MemCachedClient>();
    conf.kvclient = (std::move(client_));
    client.Init(conf);
    uuid_t uuid_key;
    uuid_t uuid_value;
    std::vector<std::pair<std::string, std::string>> key_vec;
    std::vector<std::thread> workers;
    int i;
    for (i = 0; i < 5; i++) {
        workers.emplace_back([&]() {
            char key[64];
            char value[64];
            uuid_generate(uuid_key);
            uuid_unparse(uuid_key, key);
            uuid_generate(uuid_value);
            uuid_unparse(uuid_value, value);
            std::pair<std::string, std::string> pair;
            pair.first = key;
            pair.second = value;
            key_vec.push_back(pair);
            client.Set(key, value, strlen(value));
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::string v;
            client.Get(key, &v);
            std::cout << "key = " << key << "value = " << v;
        });
    }
    for (auto& iter : workers) {
        iter.join();
    }
    client.Uninit();
    return 0;
}
