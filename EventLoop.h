// @Author Chenlujiu
// @Email 434425042@qq.com
#pragma once
#include "base/Thread.h"
#include "Epoll.h"
#include "base/Logging.h"
#include "Channel.h"
#include "base/CurrentThread.h"
#include "Util.h"
#include <vector>
#include <memory>
#include <functional>

#include <iostream>
using namespace std;


class EventLoop
{
public:
    typedef std::function<void()> Functor;
    EventLoop();
    ~EventLoop();
    void loop();
    void quit();
    void runInLoop(Functor&& cb);						
    void queueInLoop(Functor&& cb);
    bool isInLoopThread() const 
	{ 
		return threadId_ == CurrentThread::tid();
	}
    void assertInLoopThread()
    {
        assert(isInLoopThread());
    }
    void shutdown(shared_ptr<Channel> channel)
    {
        shutDownWR(channel->getFd());
    }
    void removeFromPoller(shared_ptr<Channel> channel)
    {
        //shutDownWR(channel->getFd());
        poller_->epoll_del(channel);
    }
    void updatePoller(shared_ptr<Channel> channel, int timeout = 0)
    {
        poller_->epoll_mod(channel, timeout);
    }
    void addToPoller(shared_ptr<Channel> channel, int timeout = 0)
    {
        poller_->epoll_add(channel, timeout);
    }
    
private:
    // 声明顺序 wakeupFd_ > pwakeupChannel_
    bool looping_;									//判断是否开始执行loop()函数
    shared_ptr<Epoll> poller_;						//EventLoop拥有的IO复用类的指针，用于调用复用函数，返回就绪Channel
    int wakeupFd_;									//在构造函数里生成的eventfd用来从poll里唤醒eventloop
    bool quit_;
    bool eventHandling_;							//判断EventLoop是否在处理Channel的标志
    mutable MutexLock mutex_;						//用于保护回调函数容器，以达到线程安全的目的
    std::vector<Functor> pendingFunctors_;			//保存用户回调的容器，被mutex_所保护
    bool callingPendingFunctors_;					//判断EventLoop是否在处理用户回调的标志，用于判断当前所调用的queueInLoop是否需要预唤醒EventLoop
    const pid_t threadId_; 
    shared_ptr<Channel> pwakeupChannel_;			//wakeupFd_的Channel
    
    void wakeup();									//唤醒函数，具体做法是向wakeupFd_中写入1byte数据，使其变得可读
    void handleRead();								//wakeupFd_被写入后，用来读取数据，绑定到pwakeupChannel_
    void doPendingFunctors();						//在处理完所有Channel后，处理用户回调的函数
    void handleConn();								
};
