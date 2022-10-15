
#include <stdio.h>
#include <sys/time.h>

#include <cstdint>
#include <ctime>
#include <variant>
#include <vector>
template <typename T>
struct Base {
 public:
  void Print() { return static_cast<T*>(this)->Printimp(); }

 private:
  friend T;
};

struct Child_1 : Base<Child_1> {
  friend Base<Child_1>;

 private:
  void Printimp() { printf("Child_1\n"); }
};

struct Child_2 : Base<Child_2> {
  friend Base<Child_2>;

 private:
  void Printimp() { printf("Child_2\n"); }
};

struct Child_3 : Base<Child_3> {
  friend Base<Child_3>;

 private:
  void Printimp() { printf("Child_3\n"); }
};

struct Child_4 : Base<Child_4> {
  friend Base<Child_4>;

 private:
  void Printimp() { printf("Child_4\n"); }
};

using container_type =
    std::vector<std::variant<Child_1, Child_2, Child_3, Child_4>>;

class Manager {
 public:
  template <typename Type>
  void init(Type& type) {
    vec_actor.push_back(type);
  }

  void run() {
    for (auto& base_iter : vec_actor) {
      std::visit([](auto& act) { act.Print(); }, base_iter);
    }
  }

 private:
  container_type vec_actor;
};

class Timer {
 public:
  void Now() {
    gettimeofday(&time_now, nullptr);
    now_data_sec = time_now.tv_sec;
    now_data_usec = time_now.tv_usec;
  }
  void End() {
    gettimeofday(&time_now, nullptr);
    printf("sec: %lu usec: %lu\n", time_now.tv_sec - now_data_sec,
           time_now.tv_usec - now_data_usec);
  }
  struct timeval time_now;
  uint64_t now_data_sec;
  uint64_t now_data_usec;
};

int main() {
  Manager manager;
  Child_1 child1;
  manager.init(child1);
  Timer timer;
  timer.Now();
  int i = 1;
  for (; i < 100000; i++) {
    manager.run();
  }
  timer.End();
  return 0;
}
