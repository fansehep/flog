#include <iostream>
#include <string.h>
#include <string>
#include <fcntl.h>
#include <sys/fcntl.h>
#include <unistd.h>

void SourceCodeTo(int fd) {

}

// loginit 传入配置文件的路径
int main(int argc, char* argv[]) {
  printf("%d %s %s\n", argc, argv[0], argv[1]);
  //* argv[1] 为配置文件的路径
  int configfilefd = open(argv[1], O_RDONLY);
  if(configfilefd <= 0) {
    printf("Error!, can not open the configfile, please check it right!\n");
    exit(-1);
  }
  //* 
  char buf[512] = {0};
  read(configfilefd, buf, sizeof(buf));
  //* 检查 flog.conf 的 logpath(所需要生成的日志目录是否正确)
  char logpath[256];
  for(int i = 0; i < 512; ++i) {
    logpath[i] = buf[11 + i];
    if(buf[11 + i] == '"' ) {
      buf[11 + i] = '\0';
    }
  }
  int access_val = access(logpath, R_OK | W_OK);
  if(access_val == -1) {
    printf("errno, the flog.conf logpath is errno!");
    if(errno == )
  }
  return 0;
}