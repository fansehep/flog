#include "AsyncLogThread.h"
#include "Logging.h"

namespace flog {

void AsyncLogThread::Push(Logger* log) {
  mutex_.lock();
  threadloggers_.push_back(log);
  mutex_.unlock();
}

void AsyncLogThread::SyncLogFile() {
  while(true) {
    mutex_.lock();
    for(auto& iter : threadloggers_) {
      iter->GetStream().GetBuffer().Async();
    }
    mutex_.unlock();
    sleep(sleepuseconds_);
  }
}

}