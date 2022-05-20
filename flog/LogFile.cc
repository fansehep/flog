#include "LogFile.h"


namespace flog {

LogFile::LogFile() {
    char logfilename[128];
    gettimeofday(&now, nullptr);
    auto t_second = now.tv_sec * 1000 * 1000 + now.tv_usec;
    t_second /= (1000 * 1000);
    gmtime_r(&t_second, &tm_time);
    snprintf(logfilename, sizeof(logfilename), "t_%4d%02d%02d.log", tm_time.tm_year + 1900, \
      tm_time.tm_mon + 1, tm_time.tm_mday);
    yesterday = tm_time.tm_mday;
    fd_ = open(logfilename, O_CREAT | O_RDWR, 0777);
    if(fd_ <= 0) {
      printf("errno!, can not create file!\n");
      exit(-1);
    }
    filename_ = logfilename;
}

void LogFile::Write(const char* str, int len) {
  write(fd_, str, len);
}




}