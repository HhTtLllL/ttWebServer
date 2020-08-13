#include "HttpResponse.h"
#include "../net/Buffer.h"
#include <stdio.h>

using namespace tt;
using namespace tt::net;


void HttpResponse::appendToBuffer(Buffer *output) const{

	char buf[32];

	snprintf(buf, sizeof(buf), "HTTP/1.1 %d", m_statusCode);

	output->append(buf);
	output->append(m_statusMessage);
	output->append("\r\n");

	if(m_closeConnection){
	
		    //如果是短连接,不需要告诉游览器 Content-length ,游览器也能正确处理
		    // 短连接 不存在 粘包问
		    output->append("Connection: close\r\n");
	}else{
	
		//实体的长度
		snprintf(buf, sizeof(buf), "Content-Length: %zd\r\n", m_body.size());
		output->append(buf);
		output->append("Connection: Keep-Alive\r\n");
	}

	//headr 列表
	
	for(const auto& header : m_headers){
	
		output->append(header.first);
		output->append(": ");
		output->append(header.second);
		output->append("\r\n");
	}

	//头部和实体部分应该有一个空行
	output->append("\r\n");  //header 和 body 之间的空行
	output->append(m_body);
}

