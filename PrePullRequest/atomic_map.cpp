#include <mutex>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <algorithm>


template <typename T>
class ThreadSafeHolder {
private:
  struct LockHolder {
    bool islock_;
    T& valueref_;
    std::mutex& mtx_;
    explicit LockHolder(T& val, std::mutex& mtx) :
      islock_(true), valueref_(val), mtx_(mtx) { mtx_.lock();}

    ~LockHolder() {
      Unlock();
    }

    void Unlock() {
      islock_ = false;
      mtx_.unlock();
    }

    T& operator * () {
      return valueref_;
    }

    T* operator -> () {
      return &valueref_;
    }
  };
public:
  ThreadSafeHolder() = default;
  ThreadSafeHolder(const T& val) : valref_(val) {}
  LockHolder GetLockHolder() {
    return LockHolder(valref_, mtx_);
  }
private:
  std::mutex mtx_;
  T valref_;
};



int main () {
  std::vector<std::thread> workers;
  ThreadSafeHolder<int> a(0);
  int i = 0;
  for (i = 0; i < 5; i++) {
    workers.emplace_back([&](){
      auto b = a.GetLockHolder();
      for (int j = 0; j < 100000000; j++) {
        (*b)++;
      }
    });
  }
  for (auto& iter : workers) {
    iter.join();
  }
  auto b = a.GetLockHolder();
  printf("%d\n", (*b));
  return 0;
}
