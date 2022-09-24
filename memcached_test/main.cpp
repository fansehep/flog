#include <iostream>

#include "kvclient_manager.h"


// simple_test pass
int main() {
    curvefs::client::KvClientManager kvcli;
    const std::vector<std::pair<std::string, uint32_t>> conf = {
        {"127.0.0.1", 18080}
    };
    kvcli.Init(conf, 5, 32);
    char value[] = "1234";
    kvcli.Set("123", value, strlen(value));
    sleep(2);
    std::string res;
    kvcli.Get("123", &res);
    std::cout << res;
    return 0;
}
