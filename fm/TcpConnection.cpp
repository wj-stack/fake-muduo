//
// Created by wyatt on 2022/4/21.
//

#include "TcpConnection.h"
#include <unistd.h>

TcpConnection::TcpConnection(EventLoop *loop_, int fd_, int id_) : loop(loop_), fd(fd_), channel(loop, fd), id(id_) {

//    WYATT_LOG_ROOT_DEBUG() << "TcpConnection";

}

void TcpConnection::Send(const std::string &message) {

    size_t n = ::write(fd, message.c_str(), message.size());
    if (n < message.length())
    {
        // 说明写入不足，需要将剩下的放在buffer里面
        char* p = const_cast<char *>(message.c_str() + n);
        outputBuffer.append(p, message.size() - n); // 添加到buffer里面
        channel.setWriteCallBack(std::bind(&TcpConnection::WriteHandle, this, shared_from_this()));
        channel.enableWriting(); // 关注写事件，啥时候能写了，就 写 并且 关闭写事件
    }
}

void TcpConnection::WriteHandle(const TcpConnection::ptr &ptr) {
    // 事件可写
    size_t n = ::write(fd, outputBuffer.begin() + outputBuffer.getReadIndex(), outputBuffer.getReadable());
    outputBuffer.retrieve(n);
    if (outputBuffer.getReadable() == 0)
    {
        // 说明写完了 ， 取消关注写事件
        channel.disableWriting();
    }
}
void TcpConnection::ReadHandel(const TcpConnection::ptr & ptr) {

    auto &buffer = ptr->getInputBuffer();
    int n = buffer.read(ptr->getFd());
    if (n > 0) {
        if (readCallBack)readCallBack(ptr, buffer, n); // 执行客户回调
    }else if(n == 0)
    {
        CloseHandel(ptr);
    }else{
        ErrorHandel(ptr);
    }

}


void TcpConnection::setConnectCallBack(const TcpConnection::ConnectCallBack &callBack) {
    connectCallBack = callBack;
}

void TcpConnection::setReadCallBack(const TcpConnection::ReadCallBack &callBack) {
    readCallBack = callBack;
}

void TcpConnection::setCloseCallBack(const TcpConnection::CloseCallBack &callBack) {
    closeCallBack = callBack;
}



void TcpConnection::setWriteCallBack(const TcpConnection::WriteCallBack &callBack) {
    writeCallBack = callBack;
}

void TcpConnection::setErrorCallBack(const TcpConnection::ErrorCallBack & callBack) {
    errorCallBack = callBack;
}

void TcpConnection::CloseHandel(const TcpConnection::ptr & ptr) {
    ptr->setState(TcpConnection::CLOSE);
    ptr->getChannel()->disableAll();
    ptr->getChannel()->remove();
    if (closeCallBack)closeCallBack(ptr);
    ::close(ptr->getFd());
}

void TcpConnection::ErrorHandel(const TcpConnection::ptr & ptr) {
    ptr->setState(TcpConnection::CLOSE);
    ptr->getChannel()->disableAll();
    ptr->getChannel()->remove();
    if (errorCallBack)errorCallBack(ptr);
    ::close(ptr->getFd());
}



