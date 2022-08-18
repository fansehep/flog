#include <benchmark/benchmark.h>
#include <list>
#include <random>
#include <unordered_map>
#include <uuid/uuid.h>
#include <unistd.h>

using std::string;

class LRUCache_t {
public:
  struct Item {
    const string *key;
    string value;
  };
  LRUCache_t() = default;

  void Put(const string &key, const string &value) {
    Item item{nullptr, value};
    ll_.push_front(item);
    map_[key] = ll_.begin();
    ll_.begin()->key = &(map_.find(key)->first);
  }

private:
  std::list<Item> ll_;
  std::unordered_map<string, std::list<Item>::iterator> map_;
};

double GetMemoryUse() {
  char file_name[64] = {0};
  FILE *fd;
  char line_buff[512] = {0};
  sprintf(file_name, "/proc/%d/status", getpid());
  fd = fopen(file_name, "r");
  if (nullptr == fd) {
    return 0;
  }
  char name[64];
  int vmrss = 0;
  for (int i = 0; i < 21; i++) {
    fgets(line_buff, sizeof(line_buff), fd);
  }
  fgets(line_buff, sizeof(line_buff), fd);
  sscanf(line_buff, "%s %d", name, &vmrss);
  fclose(fd);
  // cnvert VmRSS from KB to MB
  return vmrss / 1024.0;
}

static void test_2(benchmark::State &state) {
  for (auto _ : state) {
    LRUCache_t cache;
    const int N = 1000000;
    char key[64];
    char value[64];
    uuid_t uuid_key;
    uuid_t uuid_value;
    for (int i = 0; i < N; i++) {
      uuid_generate(uuid_key);
      uuid_unparse(uuid_key, key);
      uuid_generate(uuid_value);
      uuid_unparse(uuid_value, value);
      cache.Put(key, value);
    }
    printf("CurProgress Cost %f MB Most!\n", GetMemoryUse());
  }
}

BENCHMARK(test_2);
BENCHMARK_MAIN();