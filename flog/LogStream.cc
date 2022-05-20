#include "LogStream.h"


namespace flog {

static void defaultAbortFunc() {
  printf("Bye Bye, I am Abort!\n");
}
LogStream::LogStream() : func_(defaultAbortFunc) {}
//* 产生二进制日志的真实地方
//* 这里主要使用 va_list 和 va_start va_arg 来实现可变参数
//* 单条日志的格式 : 
//* |uint64_t time | string filename \n | int line | ... \r\n
//* 后续的 ... flog 只记录可变参数, 这里还原可能有点麻烦
//* filename + line 这里对应的全部已知的字符串，且必定唯一，用它来标记已知的字符串即可
void LogStream::Take(const char* file, int line, uint8_t loglevel, const \
  char* strlog, ...) {
      printf("%s\n", strlog);
      auto len = 0;
      struct timeval now;
      gettimeofday(&now, nullptr);
      uint64_t time = now.tv_sec * 1000 * 1000 + now.tv_usec;
      printf("%ld %s %d %d \n", time, file, line, loglevel);
      memcpy(buf, &time, sizeof(time));
      len += sizeof(struct timeval);
      memcpy(buf + len, file, strlen(file));
      len += strlen(file);
      memcpy(buf + len, &line, sizeof(line));
      len += sizeof(line);
      memcpy(buf + len, &loglevel, sizeof(loglevel));
      len += sizeof(loglevel);
      va_list argptr;
      va_start(argptr, 32);
      for(size_t i = 0; i < strlen(strlog); ++i) {
        if(strlog[i] == '%') {
          if(strlog[i + 1] == 'd') {
            auto val = va_arg(argptr, int);
            printf("int : %d\n", val);
            memcpy(buf + len, &val, sizeof(int));
            len += sizeof(int);
          } else if(strlog[i + 1] == 'c') {
            auto val = va_arg(argptr, int);
            printf("char : %c\n", val);

            memcpy(buf + len, &val, sizeof(char));
            len += sizeof(char);
          } else if(strlog[i + 1] == 's') {
            auto val = va_arg(argptr, char*);
            memcpy(buf + len, &val, sizeof(char));
            printf("string : %s\n", val);

            len += strlen((const char*) argptr);
          } else if(strlog[i + 1] == 'l') {
            auto val = va_arg(argptr, long long);
            printf("long long : %lld\n", val);
            memcpy(buf + len, &val, sizeof(long long));
            
            len += sizeof(long long);
          } else if(strlog[i + 1] == 'f') {
            auto val = va_arg(argptr, double);
            printf("float : %lf\n", val);
            memcpy(buf + len, &val, sizeof(double));
            len += sizeof(float);
          } else if(strlog[i + 1] == 'l' && strlog[i + 2] == 'f') {
            auto val =va_arg(argptr, double);
            printf("double : %lf\n", val);
            memcpy(buf + len, &val, sizeof(double));
            len += sizeof(double);
          }
        }
      }
      va_end(argptr);
      buf[len + 1] = '\r';
      buf[len + 2] = '\n';
      len += 2;
      printf("%s\n", buf);
      buffer_.Append(buf, len);
      if(loglevel == 6) {
        func_();
      }
}
}