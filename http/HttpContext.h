#ifndef TT_HTTP_HTTPCONTEXT_H
#define TT_HTTP_HTTPCONTEXT_H

#include "../base/copyable.h"
#include "HttpRequest.h"

namespace tt
{

namespace net
{

class Buffer;

//http 解析类
class HttpContext : public tt::copyable{

public:
	enum HttpRequestParseState{

		kExpectRequestLine,
		kExpectHeaders,
		kExpectBody,
		kGotAll,
	};

	HttpContext()
		:m_state(kExpectRequestLine){ //初始  状态
		
		}

	//TODO Timestamp
	bool parseRequest(Buffer* buf);

	bool gotAll() const { return m_state == kGotAll; }

	void reset(){
	
		m_state = kExpectRequestLine;
		HttpRequest dummy;

		m_request.swap(dummy);
	}

	const HttpRequest& request() const{
	
		return m_request;
	}

	HttpRequest& request() { return m_request; }


private:

	bool processRequestLine(const char* begin, const char* end);
	
	HttpRequestParseState m_state;
	HttpRequest m_request; //http 请求



};





}//net

}//tt











#endif //TT_HTTP_HTTPCONTEXT_H
