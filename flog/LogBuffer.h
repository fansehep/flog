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
    : buf_(Size) 
      syncline_(0.66)
      changefile_(false){
  } 
  
  void Append(const char* str, int len);
  
  void Async();
  
  LogFile& GetLogFile() {
    return this->file_;
  }
  void SetAsyncLine(double line) {
    this->syncline_ = line;
  }
  double GetAsyncLine() {
    return this->syncline_;
  }
  void ChangeFile() {
    changefile_ = true;
  }
  void ChangeLogFileTime(int time) {
    
  }
private: 
  struct buffer {
    char* ptr;
    int size_;
    int cur_;
    buffer(int size = KMIDDLEBUFFERSIZE) 
    : size_(size), 
      cur_(0) {
      ptr = new char[size];
    }
    ~buffer() {
      if(ptr) {
        delete ptr;
      }
    }
    void Write(const char* buf, int len);
  };
  //* 默认不直接刷盘，需要到达一定水平线，才会去刷盘, 默认是 2/3
  double syncline_;
  std::mutex mutex_;
  static buffer buf_;
  LogFile file_;
  bool changefile_;
};
}
#endif