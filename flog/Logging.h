#ifndef _FLOG_LOGGING_H_
#define _FLOG_LOGGING_H_

#include "AsyncLogThread.h"
#include "Logger.h"

namespace flog {

AsyncLogThread logthread;
thread_local Logger logger(&logthread); 
}
#define LOG_TRACE(str, ...) flog::logger.GetStream().Take(__FILE__, __LINE__, flog::Logger::LOG_LEVEL::TRACE, str, ##__VA_ARGS__)
#define LOG_INFO(str, ...) flog::logger.GetStream().Take(__FILE__, __LINE__, flog::Logger::LOG_LEVEL::INFO, str, ##__VA_ARGS__)
#define LOG_DEBUG(str, ...) flog::logger.GetStream().Take(__FILE__, __LINE__, flog::Logger::LOG_LEVEL::DEBUG, str, ##__VA_ARGS__)
#define LOG_WARN(str, ...) flog::logger.GetStream().Take(__FILE__, __LINE__, flog::Logger::LOG_LEVEL::WARN, str, ##_VA_ARGS__)
#define LOG_ERROR(str, ...) flog::logger.GetStream().Take(__FILE__, __LINE__, flog::Logger::LOG_LEVEL::ERROR, str, ##_VA_ARGS__)
#define LOG_FATAL(str, ...) flog::logger.GetStream().Take(__FILE__, __LINE__, flog::Logger::LOG_LEVEL::FATAL, str, ##_VA_ARGS__)
#define LOG_OFF(str, ...) flog::logger.GetStream().Take(__FILE__, __LINE__, flog::Logger::LOG_LEVEL::OFF, str, ##_VA_ARGS__)

#endif
