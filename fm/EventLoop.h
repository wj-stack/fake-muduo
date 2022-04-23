//
// Created by Administrator on 2022/4/23.
//

#ifndef FM_EVENTLOOP_H
#define FM_EVENTLOOP_H


#include <thread>
#include "noncopyable.h"
#include "Channel.h"
#include "Poller.h"
#include "spdlog/spdlog.h"
class EventLoop : public noncopyable{
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void quit();
    bool isInLoopThread() const;
    void runInLoop(const Channel::EventCallBack &callBack);
    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);
private:
    pid_t threadId_;
    bool looping_;
    Poller poller;
    bool quit_ = false;
    std::vector<Channel::EventCallBack> callbacks_;
    int wakeFd_;
    Channel wakeChannel_;
    std::mutex mutex_;
    static int createEventFd();

    void wakeUp() const;
    void handleRead() const; // 处理唤醒，防止一直唤醒

};


#endif //FM_EVENTLOOP_H
