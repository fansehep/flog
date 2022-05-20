#ifndef _FLOG_LOGGER_H_
#define _FLOG_LOGGER_H_
#include <string.h>
#include <memory>
#include <stdio.h>
#include "LogStream.h"

namespace flog {
class AsyncLogThread;
class Logger {
public:
//* 经过测试, thread_local 所标记的对象不会在线程结束后所消失，仍然存在
//* 所以这里不需要使用 shared_ptr 来管理对象的生命周期
  Logger(AsyncLogThread* logthread);
  ~Logger() = default;
  enum LOG_LEVEL {
    TRACE,
    INFO,
    DEBUG,
    WARN,
    ERROR,
    FATAL,
    OFF,
  };
  LogStream& GetStream() {
    return stream_;
  }
private:
  LogStream stream_;
};
}



#endif