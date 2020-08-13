#include "../net/Buffer.h"
#include "HttpContext.h"

using namespace tt;
using namespace tt::net;

bool HttpContext::processRequestLine(const char* begin, const char* end){

	bool succeed = false;

	const char* start = begin;

	const char* space = std::find(start, end,' '); //查找空格的位置

	if(space != end && m_request.setMethod(start, space)){ //解析请求方法

		start = space + 1;
		space = std::find(start, end, ' ');
		
		if(space != end){
		
			const char* question = std::find(start, space, '?');
			if(question != space ){
			
				m_request.setPath(start, question); //解析路径
				m_request.setQuery(question, space);
			}else{
			
				m_request.setPath(start, space);
			}

			start = space + 1;

			succeed = end - start == 8 && std::equal(start, end - 1, "HTTP/1.");

			if(succeed){
			
				if(*(end - 1) == '1'){
				
					m_request.setVersion(HttpRequest::kHttp11);
				}else if(*(end - 1) == '0'){
				
					m_request.setVersion(HttpRequest::kHttp10);
				}else succeed = false;
			}
		}
	}

	return succeed;
}


//解析请求
bool HttpContext::parseRequest(Buffer* buf){

	bool ok = true;
	bool hasMore = true;

	//状态机
	
	while(hasMore){
	
		if(m_state == kExpectRequestLine) { //处于解析请求行的状态
		
			const char* crlf = buf->findCRLF();

			if(crlf){
			
				ok = processRequestLine(buf->peek(), crlf);
				if(ok){
				
					//m_request.setR 请求时间
					buf->retrieveUntil(crlf + 2);
					m_state = kExpectHeaders;
				}else hasMore = false;
			}else{
			
				hasMore = false;
			}

		}else if(m_state == kExpectHeaders) { //解析headers
		
			const char* crlf = buf->findCRLF();
			if(crlf){
				  
				const char* colon = std::find(buf->peek(), crlf, ':');  //查找 冒号 所在位置
			
				if(colon != crlf) m_request.addHeader(buf->peek(), colon, crlf);
				else{
			
					m_state = kGotAll;
					hasMore = false;
				}

				buf->retrieveUntil(crlf + 2);
		
			}else hasMore = false;
		

		}else if(m_state == kExpectBody) { //当前不支持 请求中 带body 
		
		}
	}

	return ok;
}
