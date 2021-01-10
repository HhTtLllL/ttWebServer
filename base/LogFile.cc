#include "LogFile.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include "FileUtil.h"


tt::LogFile::LogFile(const std::string& basename, int flushEveryN)
	:m_basename(basename),
	m_flushEveryN(flushEveryN),
	m_count(0),
	m_mutex(new MutexLock){
	
	m_file.reset(new FileUtil::AppendFile(basename));	
}

tt::LogFile::~LogFile(){}

void tt::LogFile::append(const char* logline, int len){

	if(m_mutex){

		MutexLockGuard lock(*m_mutex);
		append_unlocked(logline,len);
	}else{

		append_unlocked(logline,len);
	}
}

void tt::LogFile::flush(){
	if(m_mutex){

		MutexLockGuard lock(*m_mutex);
		m_file->flush();
	}else{

		m_file->flush();
	}
}

void tt::LogFile::append_unlocked(const char* logline, int len){

	m_file->append(logline, len);

	++m_count;
	if(m_count >= m_flushEveryN){

		m_count = 0;
		m_file->flush();
	}
}