#ifndef _FLOG_LOGGER_H_
#define _FLOG_LOGGER_H_
#include <string.h>
#include <stdio.h>
#include "LogStream.h"

namespace flog {
class AsyncLogThread;
class Logger {
public:

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