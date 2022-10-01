#include <algorithm>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <thread>
#include <vector>

template <typename T> class ThreadSafeHolder {
private:
  struct LockHolder {
    bool islock_;
    T &valueref_;
    std::mutex &mtx_;
    explicit LockHolder(T &val, std::mutex &mtx)
        : islock_(true), valueref_(val), mtx_(mtx) {
      mtx_.lock();
    }

    ~LockHolder() { Unlock(); }

    void Unlock() {
      islock_ = false;
      mtx_.unlock();
    }

    T &operator*() { return valueref_; }

    T *operator->() { return &valueref_; }
  };

public:
  ThreadSafeHolder() = default;
  ThreadSafeHolder(const T &val) : valref_(val) {}

  template <typename... Args>
  ThreadSafeHolder(Args &&...args) : valref_(std::forward<Args>(args)...) {}

  LockHolder GetLockHolder() { return LockHolder(valref_, mtx_); }

private:
  std::mutex mtx_;
  T valref_;
};

int main() {
  std::vector<std::thread> workers;
  ThreadSafeHolder<std::vector<int>> vec(100, 1);
  int i = 0;
  for (i = 0; i < 5; i++) {
    workers.emplace_back([&]() {
      auto b = vec.GetLockHolder();
      for (auto &iter : *b) {
        printf("%d ", iter);
      }
      printf("\n");
      b->push_back(rand());
    });
  }

  for (auto &iter : workers) {
    iter.join();
  }

  auto b = vec.GetLockHolder();
  for (auto iter : *b) {
    printf("%d ", iter);
  }
  return 0;
}
