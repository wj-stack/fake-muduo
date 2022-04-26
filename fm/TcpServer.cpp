//
// Created by wyatt on 2022/4/21.
//

#include "TcpServer.h"

#include <memory>
#include "Buffer.h"
#include <unistd.h>
int TcpServer::s_id = 0;

TcpServer::TcpServer(EventLoop *l, InetAddress *inetAddress) : loop(l), acceptor(loop, inetAddress) {

}

void TcpServer::start() {

    acceptor.setNewConnectCallBack(std::bind(&TcpServer::handelAccept, this, std::placeholders::_1, std::placeholders::_2));

    acceptor.listen();
}

void TcpServer::setConnectCallBack(const TcpConnection::ConnectCallBack &callBack) {
    connectCallBack = callBack;
}

void TcpServer::setReadCallBack(const TcpConnection::ReadCallBack &callBack) {
    readCallBack = callBack;
}

void TcpServer::setCloseCallBack(const TcpConnection::CloseCallBack &callBack) {
    closeCallBack = callBack;
}

void TcpServer::setErrorCallBack(const TcpConnection::ErrorCallBack &callBack) {
    errorCallBack = callBack;
}

void TcpServer::handelAccept(int fd, const InetAddress & inetAddress) {

    auto ptr = std::make_shared<TcpConnection>(loop, fd, ++s_id);
    Channel &channel = *ptr->getChannel();
    channel.enableReading();
    channel.setReadCallBack(std::bind(&TcpConnection::ReadHandel, ptr.get(), ptr)); // 先转发给readHandel然后转发给客户
    channel.setWriteCallBack(std::bind(&TcpConnection::WriteHandle, ptr.get(),ptr)); // 先转发给readHandel然后转发给客户
    // 这里只关注了读写，close 和 error 都在写事件里面体现了
//    channel.setErrorCallBack(std::bind(&TcpConnection::CloseHandel, ptr.get(),ptr)); // 先转发给readHandel然后转发给客户
    ptr->setConnectCallBack(connectCallBack);
    ptr->setWriteCallBack(writeCallBack);
    ptr->setReadCallBack(readCallBack);
    ptr->setCloseCallBack(closeCallBack);
    ptr->setErrorCallBack(errorCallBack);
    Clients[s_id] = ptr;
    ptr->setState(TcpConnection::CONNECTED);
    if (connectCallBack){
        connectCallBack(ptr); // 调用Connect回调函数
    }
}



