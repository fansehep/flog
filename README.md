### flog
- flog 是一个高性能的C++二进制日志库, 其拥有着让人恐惧的性能。

- 设计描述 :
  - flog 使用 thread_local 关键词，每个线程都拥有一个Logger, LoggerBuffer 含有一个LogBuffer, 每个LogBuffer 都对应拥有一个 LogFile. 
    - Logger 在构造时，就加入 AsyncLogThread 管理.
  - 异步的AsyncLogThread会定期的遍历所有的的 LogBuffer, 将他们依次刷到磁盘中。
  - flog 不记录已知日志，且flog 是二进制的. flog 不记录已知日志的原因，我认为在普通的日志库中，大量的已知的字符串占据了大量的磁盘 IO，而且日志不是时时刻刻都需要使用的，当出现问题才会使用，所以我对 flog 的设计就是不记录已知字符串，只有当你需要使用日志时候才会去看日志。
  
 - 使用方法 
   - 将 flog 打包成静态链接库，在使用前，请使用 FLogInit 程序扫描您的源码目录，LogInit会生成一份 floginit.txt
   
   - 将 flog 加入到您的源码树中，在使用前，仍然需要 使用 FLogInit 生成floginit.txt
   
   - flog 日志库所生成的日志文件是不可读的，这里请使用 FlogFormit 程序配置文件，格式化所有的二进制日志文件. 
   
     
     
     ```c++
     #include <Logging.h>
     int main(int argc, char** argv) {
         LOG_TRACE("The Peer Connection %s is UNKOWN", IP.c_str());
         return 0;
     }
     ```
     
     

