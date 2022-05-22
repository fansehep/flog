//#include "Logging.h"
//#include "Logging.h"
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <fcntl.h>

int main(void) {
  char* str_1 = "123123123";
  LOG_INFO("Hello, world!%d\n", 1);
  char* p = strstr(str_1, "123");
  printf("%c %d", *p, p - str_1);
  return 0;
}