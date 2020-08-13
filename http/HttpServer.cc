#include "HttpServer.h"
#include "../base/Logging.h"
#include "HttpContext.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include <iostream>

using namespace  tt;
using namespace tt::net;

namespace tt
{

namespace net
{

namespace detail
{

void defaultHttpCallback(const HttpRequest&, HttpResponse* resp){

	resp->setStatusCode(HttpResponse::k404NotFound);
	resp->setStatusMessage("Not Found");
	resp->setCloseConnection(true);
}

}//detail



}//net

}//tt



HttpServer::HttpServer(EventLoop* loop, const InetAddress& listenAddr, const std::string& name, TcpServer::Option option)
	: m_server(loop, listenAddr, name, option),
	m_httpCallback(detail::defaultHttpCallback){

	m_server.setConnectionCallback(std::bind(&HttpServer::onConnection, this, _1));
	m_server.setMessageCallback(std::bind(&HttpServer::onMessage, this, _1, _2));
}


void HttpServer::start(){

	LOG << "HttpServer[" << m_server.name() << "] starts listenning on " << m_server.ipPort();
  
	m_server.start();

}


void HttpServer::onConnection(const TcpConnectionPtr& conn){

	//std::cout << "建立连接 "<< std::endl;
	if(conn->connected()){
	
		conn->setContext(HttpContext()); ///TcpConnection 与一个HttpContext  绑定
	}
}


void HttpServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf){

	//std::cout << "onMessage ------" << std::endl;
	  
	//先取出 http 的上下文
	HttpContext* context = boost::any_cast<HttpContext>(conn->getMutableContext());


	//解析请求
	if(!context->parseRequest(buf)){
	
		conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
		conn->shutdown();
	}

	if(context->gotAll()){
	
		onRequest(conn, context->request());
		context->reset(); //本次请求处理完毕后，重置HttpContext，适用于长连接

	}
}


void HttpServer::onRequest(const TcpConnectionPtr& conn, const HttpRequest& req) {

	const std::string& connection = req.getHeader("Connection");
	bool close = connection == "close" || (req.getVersion() == HttpRequest::kHttp10 && connection != "Keep-Alive");

	HttpResponse response(close);
	m_httpCallback(req, &response);
	
	Buffer buf;
	response.appendToBuffer(&buf);
	conn->send(&buf);
	if (response.closeConnection())
	{
		conn->shutdown();
	}
}




































