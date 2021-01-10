#pragma once 

#include <memory>
#include <string>
#include "FileUtil.h"
#include "MutexLock.h"
#include "noncopyable.h"

namespace tt{

namespace FileUtil{

class AppendFile;

}//FileUtil

class LogFile : noncopyable{

public:

	LogFile(const std::string& basename,int flushEveryN = 1024);
	~LogFile();

	void append(const char* logline, int len);
	void flush();
	bool roolFile();

private:
	void append_unlocked(const char* logline,int len);
	const std::string m_basename;
	const int m_flushEveryN;

	int m_count;
	std::unique_ptr<MutexLock> m_mutex;
	std::unique_ptr<FileUtil::AppendFile> m_file;
};


} // tt