#ifndef MUDUO_EXAMPLES_SIMPLE_ECHO_ECHO_H
#define MUDUO_EXAMPLES_SIMPLE_ECHO_ECHO_H

#include "../net/TcpServer.h"

// RFC 862
class EchoServer
{
 public:
  EchoServer(tt::net::EventLoop* loop,
             const tt::net::InetAddress& listenAddr); //TcpServer 中需要地址

  void start();  // calls server_.start();

 private:
  void onConnection(const tt::net::TcpConnectionPtr& conn);

  void onMessage(const tt::net::TcpConnectionPtr& conn,
                 tt::net::Buffer* buf
                 );

  tt::net::TcpServer server_;
};

#endif  // MUDUO_EXAMPLES_SIMPLE_ECHO_ECHO_H
