#include "LogBuffer.h"

namespace flog {

void LogBuffer::buffer::Write(const char* buf, int len) {
  memcpy(ptr, buf, len);
  cur_ += len;
}

void LogBuffer::Append(const char* str, int len) {
  mutex_.lock();
  buf_.Write(str, len);
  mutex_.unlock();
}

void LogBuffer::Async() {
  mutex_.lock();
  if(buf_.cur_ < buf_.size_ * this->GetAsyncLine() ) {
    return ;
  }
  //* 这里不考虑更换日志文件，默认都写在一个文件中去
  //* 由于是二进制日志，所以这里使用logFormt 来划分为不同的日志文件
  file_.Write(buf_.ptr, buf_.cur_);
  buf_.cur_ = 0;
  mutex_.unlock();
}



}