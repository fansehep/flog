#include <thread>
#include <memory>
#include <stdio.h>

class Test {
public:
  Test() {
    printf("Test constructor!\n");
  }
  ~Test() {
    printf("Test disstructor!\n");
  }
  Test(const Test& tes) {
    printf("test = func!\n");
  }
  void Printf() {
    printf("I am alive!%d\n", a);
  }
  int a;
};

thread_local Test tef;
Test* tefsptr_ = nullptr;
int main(void) {
  auto th = std::make_shared<std::thread>([](){tef.Printf();tefsptr_ = &tef; });
  th->join();
  tefsptr_->Printf();
  return 0;
}