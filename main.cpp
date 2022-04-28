#include <sys/timerfd.h>
#include <csignal>
#include "spdlog/spdlog.h"
#include "fm/EventLoop.h"
#include "fm/TcpServer.h"
#include "fm/HttpServer.h"
#include "fm/TcpConnection.h"
#include "fm/EventLoopThread.h"
#include <iostream>


EventLoop eventLoop;

std::atomic_int a;


void httpCallBack(RequestHeader &requestHeader, ResponseHeader &responseHeader)
{
    auto p = requestHeader.getParam();
    spdlog::info("data:{}", requestHeader.getData());
    spdlog::info("path:{}", requestHeader.getPath());
    for(auto&v :p)
    {
        spdlog::info("n: {} v: {}", v.first, v.second);
    }
    responseHeader.setData(requestHeader.getData());
}

int main()
{


    InetAddress inetAddress(9991, false);
    HttpServer httpServer(&eventLoop, &inetAddress);
    httpServer.setHttpCallBack(httpCallBack);
    httpServer.start();

    eventLoop.loop();
}
