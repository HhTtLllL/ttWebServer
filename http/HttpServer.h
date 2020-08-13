#ifndef TT_HTTP_HTTPSERVER_H
#define TT_HTTP_HTTPSERVER_H

#include "../net/TcpServer.h"

namespace tt
{

namespace net
{

class HttpRequest;
class HttpResponse;


class HttpServer : noncopyable{





public:
	typedef std::function<void (const HttpRequest&, HttpResponse*)> HttpCallback;

	HttpServer(EventLoop* loop, const InetAddress& listenAddr, const std::string& name, TcpServer::Option option = TcpServer::kNoReusePort);

	EventLoop* getLoop() const { return m_server.getLoop(); }

	void setHttpCallback(const HttpCallback& cb)  { m_httpCallback = cb;  }

	void setThreadNum(int numThreads) { m_server.setThreadNum(numThreads); }


	void start();


private:

	void onConnection(const TcpConnectionPtr& conn);
	void onMessage(const TcpConnectionPtr& conn, Buffer* buf);
	void onRequest(const TcpConnectionPtr& , const HttpRequest& );


	//在应用层使用http协议，在传输层使用tcp协议
	
	TcpServer m_server;

	  
	//当接收到一个 http 请求, 回调 onMessage , onMessage 回调 onRequest , onRequest 回调 httpCallbac
	//在处理http 请求(即调用onRequest) 的过程中回调此函数,对请求进行具体处理
	HttpCallback m_httpCallback;

};



}// net


}//tt





#endif //TT_HTTP_HTTPSERVER_H
