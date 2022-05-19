#ifndef _FLOG_LOGBUFFER_H_
#define _FLOG_LOGBUFFER_H_

#include <stdio.h>
#include <string.h>
#include <mutex>
#include "LogFile.h"

namespace flog {
class LogBuffer {
public:
  #define KBIGBUFFERSIZE 4096 * 8
  #define KMIDDLEBUFFERSIZE 4096 * 4
  #define KSMALLBUFFERSIZE 4096 * 2

  LogBuffer(int Size = KMIDDLEBUFFERSIZE) 
    : main_buf_(Size),
      temp_buf_(Size) {
      cur_buf_ = &main_buf_;
  } 
  
  void Append(const char* str, int len);
  
  void Async();
  
  void Swapbuf();
  LogFile& GetLogFile() {
    return this->file_;
  }
private: 
  struct buffer {
    char* ptr;
    int size_;
    int cur_;
    buffer(int size = KMIDDLEBUFFERSIZE) : size_(size), cur_(0) {
      ptr = new char[size];
    }
    ~buffer() {
      if(ptr) {
        delete ptr;
      }
    }
    void Write(const char* buf, int len);
  };
  std::mutex mutex_;
  buffer* cur_buf_;
  buffer main_buf_;
  buffer temp_buf_;
  LogFile file_;
};
}
#endif