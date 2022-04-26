#include <sys/timerfd.h>
#include <csignal>
#include "spdlog/spdlog.h"
#include "fm/EventLoop.h"
#include "fm/TcpServer.h"
#include "fm/TcpConnection.h"
#include "fm/EventLoopThread.h"
#include <iostream>
#include "fm/TcpClient.h"


EventLoop eventLoop;

std::atomic_int a;
std::thread t;
void func(const TcpConnection::ptr &conn)
{
    while (conn->isConnected()) {
        spdlog::info("conn->isConnected");
        std::string  s;
        std::cin >>  s;
        conn->Send(s);
    }
}

void newConnect(const TcpConnection::ptr &conn) {
    spdlog::info("new connect");
    if (conn->isConnected()) {
        a++;
        conn->Send("hello" + std::to_string(a));
//        t = std::thread (func,conn);
//        conn->shutdown();

    }else{
        a--;
        spdlog::info("dis connect");
    }
}

void newRead(const TcpConnection::ptr &conn,Buffer& buffer , int n) {
    std::string s(buffer.begin() + buffer.getReadIndex(),n);

    std::cout << "data:" << s << " n : "<< n  << std::endl;
    conn->Send(s);

//    conn->Send(std::string('a',1024 * 1024 * 64));
    buffer.retrieve(n);
}

void Close(const TcpConnection::ptr &conn) {
    spdlog::info("Close");
}

void Error(const TcpConnection::ptr &conn) {
    spdlog::info("Error");
}


int main()
{

    spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [%@] [thread %t] %v");

    InetAddress inetAddress("127.0.0.1",9991);

    std::vector<std::shared_ptr<TcpClient>> tcpClient(100);

    for (int i = 0; i < 100; ++i) {
        tcpClient[i] = std::make_shared<TcpClient>(&eventLoop, &inetAddress);
        tcpClient[i]->setReadCallBack(newRead);
        tcpClient[i]->setConnectCallBack(newConnect);
        tcpClient[i]->setCloseCallBack(Close);
        tcpClient[i]->connect();
    }

//    TcpClient tcpClient2(&eventLoop, &inetAddress);
//    tcpClient2.setReadCallBack(newRead);
//    tcpClient2.setConnectCallBack(newConnect);
//    tcpClient2.connect();

    eventLoop.loop();
}
