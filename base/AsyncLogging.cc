#include "AsyncLogging.h"
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <functional>
#include "LogFile.h"
#include "Condition.h"
using namespace tt;

AsyncLogging::AsyncLogging(const std::string& basename, int flushInterval)
	:m_flushInterval(flushInterval),
	m_running(false),
	m_basename(basename),
	m_thread(std::bind(&AsyncLogging::threadFunc,this),"Logging"),
	m_latch(1),
	m_mutex(),
	m_cond(m_mutex),
	m_currentBuffer(new Buffer),
	m_nextBuffer(new Buffer),
	m_buffers(){

		assert(basename.size() > 1);

		m_currentBuffer->bzero();
		m_nextBuffer->bzero();
		m_buffers.reserve(16);

}

void AsyncLogging::append(const char* logline, int len){

	tt::MutexLockGuard lock(m_mutex);


	//如果当前缓冲区剩余空间足够大(可以容纳下当前日志的大小)
	
	if(m_currentBuffer->avail() > len){
		m_currentBuffer->append(logline,len);
	}else{
		//否则说明当前缓冲已经放不下这条日志，就将当前缓冲放入
		//m_bufers 数组中
		m_buffers.push_back(std::move(m_currentBuffer));

		//试图将预备好的另一块缓冲移用为当前缓冲
		if(m_nextBuffer){
			m_currentBuffer = std::move(m_nextBuffer);
		}else{
		//如果写入速度太快，一下把两块缓冲都用完了,只好重新分配
		//一块缓冲
			m_currentBuffer.reset(new Buffer);
		}

		m_currentBuffer->append(logline,len);
		m_cond.notify();
	}
	
}

void AsyncLogging::threadFunc(){

	assert(m_running == true);

	m_latch.countDown();

	LogFile output(m_basename);

	BufferPtr newBuffer1(new Buffer);
	BufferPtr newBuffer2(new Buffer);

	newBuffer1->bzero();
	newBuffer2->bzero();

	BufferVector buffersToWrite;

	//提前分配 16 个空间
	buffersToWrite.reserve(16);


	while(m_running){
		
		assert(newBuffer1 && newBuffer1->length() == 0);
		assert(newBuffer2 && newBuffer2->length() == 0);
		assert(buffersToWrite.empty());

		{
			tt::MutexLockGuard lock(m_mutex);

			if(m_buffers.empty()){
				m_cond.waitForSeconds(m_flushInterval);
			}

			m_buffers.push_back(std::move(m_currentBuffer));

			m_currentBuffer = std::move(newBuffer1);



			buffersToWrite.swap(m_buffers);

			if(!m_nextBuffer){ //替换缓冲区，这样保证前段始终有一个预备 buffer 可供调配
				m_nextBuffer = std::move(newBuffer2);
			}
		}
	

		assert(!buffersToWrite.empty());


		if(buffersToWrite.size() > 25){
		
			buffersToWrite.erase(buffersToWrite.begin() + 2,buffersToWrite.end());
		}


		for(const auto& buffer : buffersToWrite){
			
			output.append(buffer->data(),buffer->length());
		}

		if(buffersToWrite.size() > 2){
			buffersToWrite.resize(2);
		}

		if(!newBuffer1){
			assert(!buffersToWrite.empty());

			newBuffer1 = buffersToWrite.back();
			buffersToWrite.pop_back();
			newBuffer1->reset();
		}

		if(!newBuffer2){
			assert(!buffersToWrite.empty());

			newBuffer2 = buffersToWrite.back();
			buffersToWrite.pop_back();
			newBuffer2->reset();
		}

		buffersToWrite.clear();
		output.flush();
	}

	output.flush();
}










