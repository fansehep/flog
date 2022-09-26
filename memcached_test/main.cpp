#include <iostream>
#include <memory>

#include "kvclient_manager.h"
#include "memcache_client.h"
#include <string.h>

using ::curvefs::client::KvClientManager;
using ::curvefs::client::MemCachedClient;
using ::curvefs::client::KvClientManagerConfig;

// simple_test pass
int main() {
    KvClientManager<MemCachedClient> client;
    std::unique_ptr<MemCachedClient> client_ = std::make_unique<MemCachedClient>();
    std::string res;
    auto ue = client_->AddServer("127.0.0.1", 18080, &res);
    if (!ue) {
        LOG(ERROR) << "addserver error " << res;
    }
    ue = client_->PushServer();
    if (!ue) {
        LOG(ERROR) << "push server error";
    }
    auto conf = KvClientManagerConfig<MemCachedClient>();
    conf.kvclient = (std::move(client_));
    client.Init(conf);

    char str[] = "456890";
    client.Set("123", str, strlen(str));
    sleep(2);
    std::string rest;
    client.Get("123", &rest);
    std::cout << rest << std::endl;
    return 0;
}
