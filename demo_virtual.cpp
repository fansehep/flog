
#include <stdio.h>
#include <sys/time.h>
#include <vector>
#include <cstdint>
#include <ctime>
#include <variant>

class Base {
 public:
  virtual void Print() = 0;
};

class Child_1 : public Base {
 public:
  void Print() override { printf("Child_1\n"); }
};

struct Child_2 : public Base {
 public:
  void Print() override { printf("Child_2\n"); }
};

struct Child_3 : public Base {
 public:
  void Print() override { printf("Child_3\n"); }
};

struct Child_4 : public Base {
 public:
  void Print() override { printf("Child_4\n"); }
};

struct Manager {
  void init(Base* ptr) { base = ptr; }
  void run() { base->Print(); }
  Base* base;
};
Child_1 chidl;
Manager manager;

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
  manager.init(&child1);
  Timer timer;
  timer.Now();
  int i = 1;
  for (; i < 100000; i++) {
    manager.run();
  }
  timer.End();
  return 0;
}
