#ifndef TT_FILEUTL_H
#define TT_FILEUTL_H

#include "noncopyable.h"
#include <string>

namespace tt{

namespace FileUtil{

class AppendFile : noncopyable{

public:
	explicit AppendFile(std::string filename);
	~AppendFile();

	void append(const char* logline, const size_t len);
	void flush();

private:
	size_t mywrite(const char* logline,size_t len);
	FILE* m_fp;
	char m_buffer[64*1024];

};

} //FileUtil

} //tt


#endif
