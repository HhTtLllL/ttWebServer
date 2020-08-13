#ifndef TT_NET_CHANNEL_H
#define TT_NET_CHANNEL_H

#include "../base/noncopyable.h"
#include <functional>
#include <memory>

namespace tt{

namespace net{

class EventLoop;

class Channel : noncopyable{

public:
	  typedef std::function<void()> EventCallback;   //事件的回调处理
	  typedef std::function<void()> ReadEventCallback; //读事件的回调 ,多了一个时间戳


	  //一个 EventLoop可能拥有多个 channel ，但是一个channel 只能拥有一个 EventLoop
	  Channel(EventLoop* loop, int fd);
	  ~Channel();

	  //对所发生的IO 事件的处理
	  //有可读事件来, 调用channel 的 handeventl , 然后 handleEvent 调用acceptor 的 handleRead
	  void handleEvent();


	  //回调函数的注册
	  void setReadCallback(ReadEventCallback cb){ m_readCallback = std::move(cb); }
	  void setWriteCallback(EventCallback cb) { m_writeCallback = std::move(cb); }
	  void setCloseCallBack(EventCallback cb) { m_closeCallback = std::move(cb); }
	  void setErrorCallBack(EventCallback cb) { m_errorCallback = std::move(cb); }

	  int fd() const { return m_fd; }
	  int events() const { return m_events; }
	  void set_revents(int revent) { m_revents = revent; }
	  bool isNoneEvent() const { return m_events == kNoneEvent; }



	  //关注读的事件,或者加入这个事件,
	  /*updata  调用EventLoop 的  updateChannel  ,updateChannel 调用 poller的 updateChannel ,把事件注册 到 epoll中*/
	  void enableReading() { m_events |= kReadEvent; update(); }
	  void disableReading() { m_events &= ~kReadEvent; update(); }
	  void enableWriting() { m_events |= kWriteEvent; update(); }
	  void disableWriting() { m_events &= ~kWriteEvent; update(); }


	  //不关注事件了
	  void disableAll() { m_events = kNoneEvent; update();}
	  
	  bool isWriting() const { return m_events & kWriteEvent; }
	  bool isReading() const { return m_events & kReadEvent; }
  
	  
	  int index() { return m_index; }
	  void set_index(int idx) { m_index = idx; }

	  EventLoop* ownerLoop() { return m_loop; }
	  void remove();
private:
	void update();

	void handleEventWithGuard();

	static const int kNoneEvent; //没有事件发生
	static const int kReadEvent;   //可读事件或者紧急事件 POLLIN / POLLPRI (tcp的带外数据)
  	static const int kWriteEvent;  // POLLOU

	EventLoop* m_loop;   //记录这个对象所属的EventLoop
	const int  m_fd;     //文件描述符,但不负责关闭该文件描述符

	int m_events;  //关注的事件,由用户设置
	int m_revents; // it's the received event types of epoll or poll          实际返回的事件
	int m_index; // used by Poller.  表示在poll的事件数组中的序号
	
	//bool       logHup_;
	
	//std::weak_ptr<void> tie_;
  
//	bool tied_;
	bool m_eventHandling;  //是否处于事件处理中
	bool m_addedToLoop;

	//回调
	ReadEventCallback m_readCallback;
	EventCallback m_writeCallback;
	EventCallback m_closeCallback;
	EventCallback m_errorCallback;


};



}// net



}//tt


#endif
