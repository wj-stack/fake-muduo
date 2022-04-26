//
// Created by wyatt on 2022/4/21.
//

#ifndef SOCKETFRAMEWORK_TCPCONNECTION_H
#define SOCKETFRAMEWORK_TCPCONNECTION_H

#include "Channel.h"
#include "EventLoop.h"
#include "Buffer.h"

class TcpConnection : std::enable_shared_from_this<TcpConnection> {
public:

    using ptr = std::shared_ptr<TcpConnection>;
    typedef std::function<void(const TcpConnection::ptr &)> ConnectCallBack;
    typedef std::function<void(const TcpConnection::ptr &, Buffer &, size_t)> ReadCallBack;
    typedef std::function<void(const TcpConnection::ptr &)> CloseCallBack;
    typedef std::function<void(const TcpConnection::ptr &)> WriteCallBack; // 可写
    typedef std::function<void(const TcpConnection::ptr &)> ErrorCallBack;

    enum STATE {
        CONNECTING, CONNECTED, ERROR, CLOSE
    };

    TcpConnection(EventLoop *loop_, int fd_, int id_);

    int getState() const { return state; }

    void setState(STATE s) { state = s; }

    Channel *getChannel() { return &channel; }

    int getFd() const { return fd; }

    int getIndex() const { return id; }

    Buffer &getInputBuffer() { return inputBuffer; }

    void Send(const std::string& message);


public:
    void setConnectCallBack(const ConnectCallBack &connectCallBack);

    void setReadCallBack(const ReadCallBack &readCallBack);

    void setCloseCallBack(const CloseCallBack &closeCallBack);

    void setWriteCallBack(const WriteCallBack &writeCallBack);

    void setErrorCallBack(const ErrorCallBack &);

    void WriteHandle(const TcpConnection::ptr& ptr);

    void ReadHandel(const TcpConnection::ptr &);

    void CloseHandel(const TcpConnection::ptr &);



private:
    Buffer inputBuffer;
    Buffer outputBuffer; // 对与写数据来说，先尝试着一次性写完，不过写不完，那就将剩余的放在buffer里，并关注写事件
    int id;
    EventLoop *loop;
    int fd;
    Channel channel;
    STATE state = CONNECTING;

    ConnectCallBack connectCallBack;
    ReadCallBack readCallBack;
    CloseCallBack closeCallBack;
    WriteCallBack writeCallBack;
    ErrorCallBack errorCallBack;

    void ConnectHandel(const ptr &);

    void ErrorHandel(const ptr &ptr);
};


#endif //SOCKETFRAMEWORK_TCPCONNECTION_H
