#include "LogBuffer.h"

namespace flog {

void LogBuffer::buffer::Write(const char* buf, int len) {
  memcpy(ptr, buf, len);
  cur_ += len;
}

void LogBuffer::Append(const char* str, int len) {
  mutex_.lock();
  cur_buf_->Write(str, len);
  mutex_.unlock();
}

void LogBuffer::Async() {
  mutex_.lock();
  Swapbuf();
  mutex_.unlock();
  buffer* t_buf_ = (cur_buf_ == &main_buf_) ? &temp_buf_ : &main_buf_;
  file_.Write(t_buf_->ptr, t_buf_->size_);
}

void LogBuffer::Swapbuf() {
  cur_buf_ = (cur_buf_ == &main_buf_ ? &temp_buf_ : &main_buf_);
  cur_buf_->cur_ = 0;
}

}