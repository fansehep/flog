#include "Logging.h"
#include <thread>

int main(void) {
  while(true) {
    LOG_DEBUG("sorry, I want to fly %s %d", "but i can't.", 1);
    usleep(333);
  }
  return 0;
}