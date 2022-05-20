#ifndef _FLOG_LOGFILE_H_
#define _FLOG_LOGFILE_H_
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/fcntl.h>
#include <string.h>
#include <string>

namespace flog {

class LogFile {
public:
  LogFile();
  ~LogFile() {
    if(fd_ >= 0) {
      close(fd_);
    }
  }
  void Open(const char* filename) {
    fd_ = open(filename, O_RDWR | O_CREAT, 0777);
    if(fd_ <= 0) {
      printf("errno!, can not open or create file!\n");
      exit(-1);
    }
    filename_ = filename;
  }

  void Write(const char* buf, int len);
  void Close() {
    close(fd_);
  }
  int Getfd() {
    return fd_;
  }

private:
  struct tm tm_time;
  struct timeval now;
  std::string filename_;
  int fd_;
};
}
#endif