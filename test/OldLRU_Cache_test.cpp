#include <benchmark/benchmark.h>
#include <list>
#include <random>
#include <unordered_map>

using std::string;

class LRUCache {
public:
  struct Item {
    string key;
    string value;
  };
  LRUCache() = default;

  void Put(const string &key, const string &value) {
    Item item{key, value};
    ll_.push_front(item);
    map_[key] = ll_.begin();
  }

private:
  std::list<Item> ll_;
  std::unordered_map<string, std::list<Item>::iterator> map_;
};

static void test_1(benchmark::State &state) {
  LRUCache cache;
  const int N = 1000000;
  char key[64];
  char value[64];
  for (int i = 0; i < N; i++) {
    cache.Put(key, value);
  }
}

BENCHMARK(test_1);
BENCHMARK_MAIN();