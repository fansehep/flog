#include "Logging.h"
#include <thread>
#include <sys/time.h>
#include <unistd.h>
int main(void) {
  while(true) {
    LOG_DEBUG("sorry, I want to fly %s %d", "but i can't.", 1);
    sleep(333);
  }
  return 0;
}