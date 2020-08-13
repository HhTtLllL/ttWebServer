#ifndef TT_HTTP_HTTPREQUEST_H
#define TT_HTTP_HTTPREQUEST_H

#include "../base/copyable.h"
#include "../base/Common.h"
#include <map>
#include <assert.h>
#include <stdio.h>

namespace tt
{

namespace net
{

class HttpRequest : public tt::copyable{

public:
	  enum Method{

		  //kInvalid   无效的方法, 其他为 当前支持的方法
		  kInvalid, kGet, kPost, kHead, kPut, kDelete
	  };

	  //http 协议版本
	  enum Version{

		  kUnknown, kHttp10, kHttp11
	  };

	  HttpRequest()
		  :m_method(kInvalid),
		  m_version(kUnknown){
		 
		  }

	  void setVersion(Version v){ m_version = v; }

	  Version getVersion() const { return m_version; }

	  bool setMethod(const char* start, const char* end){
	  
		  assert(m_method == kInvalid );
		  std::string m(start, end);

		  if(m == "GET") m_method = kGet;
		  else if(m == "POST") m_method = kPost;
		  else if(m == "HEAD") m_method = kHead;
		  else if(m == "PUT") m_method = kPut;
		  else if(m == "DELETE") m_method = kDelete;
		  else m_method = kInvalid;


		  return m_method != kInvalid;
	  }


	  Method method() const { return m_method; }

	  //返回请求方法
	
	  const char* methodString() const{
	  
		  const char* result = "UNKNOWN";
		  switch(m_method){

			  case kGet:
				  result = "GET";
				  break;
			  case kPost:
				  result = "POST";
				  break;
			  case kHead:
				  result = "HEAD";
				  break;
			  case kPut:
        
				  result = "PUT";
				  break;
			  case kDelete:
				  result = "DELETE";
				  break;
			  default:
				  break;
		  }
		  
		  return result;
	  }

	  //设置 路径
	  void setPath(const char* start, const char* end){

		  m_path.assign(start, end);
	  }

	  const std::string& path() const { return m_path; }


	  void setQuery(const char* start, const char* end){

		  m_query.assign(start, end);
	  }

	  const std::string& query() const { return m_query; }
/*
	  //设置接受时间
	  void setReceiveTime(Timestamp t){ m_receiveTime = t; }
	  //返回接受时间
	  Timestamp receiveTime() const { return m_receiveTime; }

*/

	  //添加一个头部信息

	  void addHeader( const char* start, const char* colon, const char* end){
	  
		  std::string field(start, colon); //header 域
		  ++colon; 

		  //取出左空格
		  while(colon < end && isspace(*colon)) ++colon;

		  std::string value(colon, end); //head 值

		  //取出右空格

		  while(!value.empty() && isspace(value[value.size() - 1])) { 
			  
			  value.resize(value.size() - 1); 
		  }

		  m_headers[field] = value;
	  }


	  //获取头部信息
	  std::string getHeader(const std::string& field) const{
	  
		  std::string result;
		  std::map<std::string, std::string >::const_iterator it = m_headers.find(field);

		  if(it != m_headers.end()) { result = it->second; }

		  return result;
	  }


	  const std::map<std::string, std::string>& headers() const { return m_headers; }

	  void swap(HttpRequest& that){
	  
		  std::swap(m_method, that.m_method);
		  std::swap(m_version,that.m_version);

		  m_path.swap(that.m_path);
		  m_query.swap(that.m_query);

		  //m_receiveTime.swap(that.m_receiveTime) ;
		  m_headers.swap(that.m_headers);
	  }

private:
	Method m_method;  //请求方法
	Version m_version; //协议版本
	std::string m_path;
	std::string m_query;
	//Timestamp m_receiveTime; //其扭曲时间
	
	std::map<std::string, std::string> m_headers;  //headr 请求列表
};

}//net
}//tt













#endif //TT_HTTP_HTTPREQUEST_H
