//
// Created by wyatt on 2022/4/21.
//

#ifndef SOCKETFRAMEWORK_TCPSERVER_H
#define SOCKETFRAMEWORK_TCPSERVER_H

#include "EventLoop.h"
#include "Acceptor.h"
#include "InetAddress.h"
#include <functional>
#include <map>
#include "TcpConnection.h"
class TcpServer {
public:

    TcpServer(EventLoop *l, InetAddress *inetAddress);

    void start();

    void setConnectCallBack(const TcpConnection::ConnectCallBack &callBack);

    void setReadCallBack(const TcpConnection::ReadCallBack &callBack);

    void setCloseCallBack(const TcpConnection::CloseCallBack &callBack);

    void setErrorCallBack(const TcpConnection::ErrorCallBack &callBack);


private:


//    void ReadHandel(const TcpConnection::ptr &);
//
//    void CloseHandel(const TcpConnection::ptr &);

//    void WriteHandel(const TcpConnection::ptr &);

    static int s_id;
    std::map<int, TcpConnection::ptr> Clients;
    EventLoop *loop;
    Acceptor acceptor;

    TcpConnection::ConnectCallBack connectCallBack;
    TcpConnection::ReadCallBack readCallBack;
    TcpConnection::CloseCallBack closeCallBack;
    TcpConnection::WriteCallBack writeCallBack;
    TcpConnection::ErrorCallBack errorCallBack;

    void handelAccept(int fd, const InetAddress &inetAddress);
};


#endif //SOCKETFRAMEWORK_TCPSERVER_H
