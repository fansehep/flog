#include <benchmark/benchmark.h>
#include <unordered_map>
#include <list>
#include <random>

using std::string;

class LRUCache_t {
public:
  struct Item {
    const string* key;
    string value;
  };
  LRUCache_t() = default;

  void Put (const string& key, const string& value) {
    Item item{nullptr, value};
    ll_.push_front(item);
    map_[key] = ll_.begin();
    ll_.begin()->key = &(map_.find(key)->first);
  }
private:
  std::list<Item> ll_;
  std::unordered_map<string, std::list<Item>::iterator> map_;
};



static void test_2 (benchmark::State& state) {
  LRUCache_t cache;
  const int N = 1000000;
  char key[64];
  char value[64];
  for (int i = 0; i < N; i++) {
    cache.Put(key, value);
  }
}

BENCHMARK(test_2);
BENCHMARK_MAIN();