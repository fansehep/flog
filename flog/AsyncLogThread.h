#ifndef _FLOG_ASYNCLOGTHREAD_H_
#define _FLOG_ASYNCLOGTHREAD_H_

#include <thread>
#include <mutex>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <vector>

namespace flog {
class Logger;
class AsyncLogThread {
public:
  AsyncLogThread() : sleepuseconds_(333)
  {
    logthread_ = std::make_unique<std::thread>([this](){SyncLogFile();});
  };
  void Push(Logger* log);
  void SyncLogFile();
  void SetSleepuSeconds(uint64_t tim) {
    sleepuseconds_ = tim;
  }
private:
  std::mutex mutex_;
  std::unique_ptr<std::thread> logthread_;
  std::vector<Logger*> threadloggers_;
  uint64_t sleepuseconds_;
};
}
#endif