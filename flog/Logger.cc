#include "Logger.h"
#include "AsyncLogThread.h"

namespace flog {

Logger::Logger(AsyncLogThread* logthread) {
  logthread->Push(this);
}


}