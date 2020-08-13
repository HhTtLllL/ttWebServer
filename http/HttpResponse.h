#ifndef TT_HTTP_HTTPRESPONSE_H
#define TT_HTTP_HTTPRESPONSE_H

#include "../base/copyable.h"
#include "../base/Common.h"
#include <map>

namespace tt
{

namespace net
{

class Buffer;

class HttpResponse : public tt::copyable{

public:
	enum HttpStatusCode{
	
		 kUnknown,
		 k200Ok = 200,   // 成功
		 k301MovedPermanently = 301,  // 301 重定向,请求的页面永久性移植至另一个地址
		 k400BadRequest = 400,   //错误的请求,语法格式有错,服务器无法处理此请求
		 k404NotFound = 404,   //  请求的网页不存
	};

	explicit HttpResponse(bool close) 
		:m_statusCode(kUnknown),
		m_closeConnection(close){
		
		
		
	}


	//设置状态码
	void setStatusCode(HttpStatusCode code){ m_statusCode = code; }

	void setStatusMessage(const std::string& message) { m_statusMessage = message; }

	void setCloseConnection(bool on) { m_closeConnection = on; }

	bool closeConnection() const { return m_closeConnection; }

	void setContentType(const std::string& contentType) { addHeader("Content-Type", contentType); }

	void addHeader(const std::string& key, const std::string& value){ m_headers[key] = value; }

	void setBody(const std::string& body) { m_body = body; }

	//将HttpRespon 添加到 buffer 以便 发送给客户端
	void appendToBuffer(Buffer* output) const;


private:

	std::map<std::string, std::string> m_headers;  //header 列表
	HttpStatusCode m_statusCode; //状态吗

	//TODO http version
	
	std::string m_statusMessage; //状态码 对应的文本信息
	bool m_closeConnection; //是否关闭连接
	std::string m_body; //实体


};






}// net
}//tt







#endif //TT_HTTP_HTTPRESPONSE_H
