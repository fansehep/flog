#ifndef _FLOG_LOGSTREAM_H_
#define _FLOG_LOGSTREAM_H_

#include <string.h>
#include <string>
#include <sys/time.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>

#include "LogBuffer.h"
namespace flog {
class LogStream {
public:
  using AbortFunc = void (void);
  LogStream();

  void Take(const char* file, int line, uint8_t loglevel, const char* strlog, ...);

  void SetAbortFunc(AbortFunc* func) {
    func_ = func;
  }  
  LogBuffer& GetBuffer() {
    return buffer_;
  }
private:
  AbortFunc* func_;
  LogBuffer buffer_;
  char buf[128];
};
}
#endif