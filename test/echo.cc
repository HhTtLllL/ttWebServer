#include "echo.h"
#include "../base/Logging.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

// using namespace muduo;
// using namespace muduo::net;

//在构造函数里注册回调函数
EchoServer::EchoServer(tt::net::EventLoop* loop,
                       const tt::net::InetAddress& listenAddr)
  : server_(loop, listenAddr, "EchoServer")
{
  server_.setConnectionCallback(
      std::bind(&EchoServer::onConnection, this, _1));
  server_.setMessageCallback(
      std::bind(&EchoServer::onMessage, this, _1, _2));
}

void EchoServer::start()
{
  server_.start();
}
//  onConnection  连接建立和断开 ,, 用 connected 判断 建立 还是断开
void EchoServer::onConnection(const tt::net::TcpConnectionPtr& conn)
{
  LOG << "EchoServer - " << conn->peerAddress().toIpPort() << " -> "
           << conn->localAddress().toIpPort() << " is "
           << (conn->connected() ? "UP" : "DOWN");
}

void EchoServer::onMessage(const tt::net::TcpConnectionPtr& conn,
                           tt::net::Buffer* buf
                           )
{
  // 把消息从缓冲区中取回, 保存在 msg 中
  std::string msg(buf->retrieveAllAsString());
 /* LOG << conn->name() << " echo " << msg.size() << " bytes, "
           << "data received at " << time.toString();*/
  conn->send(msg);
}

