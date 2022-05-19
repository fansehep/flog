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
  LogFile() {
    char logfilename[128];
    struct timeval now;
    gettimeofday(&now, nullptr);
    auto t_second = now.tv_sec * 1000 * 1000 + now.tv_usec;
    t_second /= (1000 * 1000);
    struct tm tm_time;
    gmtime_r(&t_second, &tm_time);
    snprintf(logfilename, sizeof(logfilename), "%4d%02d%02d.log", tm_time.tm_year + 1900, \
      tm_time.tm_mon + 1, tm_time.tm_mday);
    fd_ = open(logfilename, O_CREAT | O_RDWR, 0777);
    if(fd_ <= 0) {
      printf("errno!, can not create file!\n");
      exit(-1);
    }
    filename_ = logfilename;
  }
  ~LogFile() {
    if(fd_ >= 0) {
      close(fd_);
    }
  }
  void Open(const char* filename) {
    fd_ = open(filename, O_RDWR | O_CREAT);
    if(fd_ <= 0) {
      printf("errno!, can not open or create file!\n");
      exit(-1);
    }
    filename_ = filename;
  }
  void Write(const char* buf, int len) {
    write(fd_, buf, len);
  }
  void Close() {
    close(fd_);
  }
  int Getfd() {
    return fd_;
  }
private:
  std::string filename_;
  int fd_;
};
}
#endif