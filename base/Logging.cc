#include "Logging.h"
#include "CurrentThread.h"
#include "Thread.h"
#include "AsyncLogging.h"
#include <assert.h>
#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

namespace tt{

static pthread_once_t once_control_ = PTHREAD_ONCE_INIT;

static AsyncLogging *AsyncLogger_;

std::string Logger::m_logFileName = "./WebServer.log";


void once_init(){
	
	AsyncLogger_ = new AsyncLogging(Logger::getLogFileName());
	AsyncLogger_->start();
}


void output(const char* msg, int len){

	pthread_once(&once_control_, once_init);
	AsyncLogger_->append(msg, len);
}

Logger::Impl::Impl(const char * fileName, int line)
	:m_stream(),
	m_line(line),
	m_basename(fileName){
	
		formatTime(); //格式化时间
}

void Logger::Impl::formatTime(){
	struct timeval tv;

	time_t time;
	char str_t[26] = {0};

	gettimeofday(&tv,NULL);

	time = tv.tv_sec;

	struct tm* p_time = localtime(&time);
	strftime(str_t,26,"%Y-%m-%d %H:%M:%S\n",p_time);

	m_stream << str_t;
	
}

Logger::Logger(const char* fileName, int line)
	:m_impl(fileName,line){
	
	}


Logger::~Logger(){
	
	m_impl.m_stream << "--" << m_impl.m_basename << " : " 
		<< m_impl.m_line << "\n";
	
	const LogStream::Buffer& buf(stream().buffer());

	output(buf.data(), buf.length());
}

}// tt