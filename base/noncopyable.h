#pragma once

namespace tt{

//不可拷贝类    c11 使用delete 关键字实现
class noncopyable{
public:
	noncopyable(const noncopyable& ) = delete;
	noncopyable& operator=(const noncopyable& ) = delete;
protected:
	noncopyable() = default;
	~noncopyable() = default;
};


}
