#include <stdio.h>
#include <string.h>


int main(void) {
  char* str_1 = "yangfan has eat";
  char* str_2 = "gfan";
  auto t_p = strstr(str_1, str_2);
  if (t_p != NULL) [[likely]] {
    printf("%s\n", t_p);
  }
  return 0;
}