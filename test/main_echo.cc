#include "test/echo.h"

#include "base/Logging.h"
#include "net/EventLoop.h"

#include <unistd.h>

// using namespace muduo;
// using namespace muduo::net;

int main()
{
  LOG << "pid = " << getpid();
  tt::net::EventLoop loop;
  tt::net::InetAddress listenAddr(2007);

  
  EchoServer server(&loop, listenAddr);
  server.start();
  loop.loop();   //在这里不断捕捉事件,一旦有事件到来就回调 Echoserver 中相应的函数

}

