#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

int LogFd;
void SourceCodeTo(const char* SourceCodePath);
void SimpleFileToLog(FILE* fptr, const char* filename);
void SourceCodeTo(const char* SourceCodePath) {
  DIR* dir_ptr = NULL;
  struct dirent* entry;
  struct stat statbuf;
  if((dir_ptr = opendir(SourceCodePath)) == NULL) {
    printf("there is some errno with your srcpath!\n");
  }
  chdir(SourceCodePath);
  while((entry = readdir(dir_ptr)) != NULL) {
    lstat(entry->d_name, &statbuf);
    if(__S_IFDIR & statbuf.st_mode) {
      //* 不记录隐藏文件
      if(entry->d_name[0] == '.') {
        continue;
      }
      SourceCodeTo(entry->d_name);
    } else {
      int pre_idx = strlen(entry->d_name);
      //* logger.cc logger.h logger.hpp logger.cpp
      bool sourecodefile = false;
      if(entry->d_name[pre_idx - 1] == 'p') {
        if(entry->d_name[pre_idx - 2] == 'p') {
          if(entry->d_name[pre_idx - 3] == 'h' || entry->d_name[pre_idx - 3] == 'c') {
            if(entry->d_name[pre_idx - 4]) {
              sourecodefile = true;
              
            }
          }
        }
      }
      if(entry->d_name[pre_idx - 1] == 'c' && entry->d_name[pre_idx - 2] == 'c' && 
        entry->d_name[pre_idx -3] == '.') {
          sourecodefile = true;
        }
        if(entry->d_name[pre_idx - 1] == 'h' && entry->d_name[pre_idx - 2] == '.') {
          sourecodefile = true;
        }
      if(sourecodefile == true) {
        FILE* file_ptr = fopen(entry->d_name, "r");
        SimpleFileToLog(file_ptr, entry->d_name);
        sourecodefile = false;
        fclose(file_ptr);
      }
    }
  }
  chdir("..");
  closedir(dir_ptr);
}
void SimpleFileToLog(FILE* fptr, const char* filename) {
  int line = 0;
  char linebuf[512];
  while(true) {
    char* reptr = fgets(linebuf, sizeof(linebuf) - 1, fptr);
    if(reptr == NULL ) {
      break;
    }
    int n = strlen(linebuf);
    char* idx_1 = strstr(linebuf, "LOG_TRACE(\"");
    char* idx_2 = strstr(linebuf, "LOG_INFO(\"");
    char* idx_3 = strstr(linebuf, "LOG_DEBUG(\"");
    char* idx_4 = strstr(linebuf, "LOG_WARN(\"");
    char* idx_5 = strstr(linebuf, "LOG_ERROR(\"");
    char* idx_6 = strstr(linebuf, "LOG_FATAL(\"");
    char* idx_7 = strstr(linebuf, "LOG_OFF(\"");
    int len = -1;
    if(idx_1 != NULL) {
      len = idx_1 - linebuf;
    } else if(idx_2 != NULL) {
      len = idx_2 - linebuf;
    } else if(idx_3 != NULL) {
      len = idx_3 - linebuf;
    } else if(idx_4 != NULL) {
      len = idx_4 - linebuf;
    } else if(idx_5 != NULL) {
      len = idx_5 - linebuf;
    } else if(idx_6 != NULL) {
      len = idx_5 - linebuf;
    }
    
    }
}

// loginit 传入配置文件的路径
int main(int argc, char* argv[]) {
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
  int i = 0;  
  for(i = 0; i < 512; ++i) {
    logpath[i] = buf[10 + i];
    if(buf[10 + i] == '"' ) {
      logpath[i] = '\0';
      break;
    }
  }
  printf("logpath : %s\n", logpath);
  int access_val = access(logpath, W_OK | R_OK);
  if(access_val == -1) {
    printf("errno, the flog.conf logpath is errno!\n");
  } else {
    printf("check logpath OK!\n");
  }
  //* 检查所给的对应源码树是否正确
  char sourcecodepath[256];
  i += (10 + 9);
  int j = 0;
  for(; i < 512; ++i, ++j) {
    sourcecodepath[j] = buf[i];
    if(buf[i] == '"') {
      sourcecodepath[j] = '\0';
      break;
    }
  }
  printf("srcpath : %s\n", sourcecodepath);
  access_val = access(sourcecodepath, W_OK | R_OK);
  if(access_val == -1) {
    printf("errno, the flog conf src is errno!\n");
  } else {
    printf("check src OK!\n");
  }
  LogFd = open("floginit.txt", O_CREAT | O_RDWR, 0777);
  printf("start to generate ... please wait\n");
  SourceCodeTo(sourcecodepath);
  close(LogFd);
  close(configfilefd);
  return 0;
}