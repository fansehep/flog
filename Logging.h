#ifndef _FLOG_LOGGING_H_
#define _FLOG_LOGGING_H_
#include <string.h>
#include <stdio.h>
#include "AsyncLogThread.h"
#include "LogStream.h"
flog::AsyncLogThread logthread;
namespace flog {
class Logger {
public:

  Logger(AsyncLogThread* logthread) {
    logthread->Push(this);
  }
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
thread_local flog::Logger logger(&logthread);
#define LOG_TRACE(str, ...) logger.GetStream().Take(__FILE__, __LINE__, flog::Logger::LOG_LEVEL::TRACE, str, ##__VA_ARGS__)
#define LOG_INGO(str, ...) logger.GetStream().Take(__FILE__, __LINE__, flog::Logger::LOG_LEVEL::INFO, str, ##__VA_ARGS__)
#define LOG_DEBUG(str, ...) logger.GetStream().Take(__FILE__, __LINE__, flog::Logger::LOG_LEVEL::DEBUG, str, ##__VA_ARGS__)
#define LOG_WARN(str, ...) logger.GetStream().Take(__FILE__, __LINE__, flog::Logger::LOG_LEVEL::WARN, str, ##_VA_ARGS__)
#define LOG_ERROR(str, ...) logger.GetStream().Take(__FILE__, __LINE__, flog::Logger::LOG_LEVEL::ERROR, str, ##_VA_ARGS__)
#define LOG_FATAL(str, ...) logger.GetStream().Take(__FILE__, __LINE__, flog::Logger::LOG_LEVEL::FATAL, str, ##_VA_ARGS__)
#define LOG_OFF(str, ...) logger.GetStream().Take(__FILE__, __LINE__, flog::Logger::LOG_LEVEL::OFF, str, ##_VA_ARGS__)


#endif