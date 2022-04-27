cmake_minimum_required(VERSION 3.22)
project(fm)

set(CMAKE_CXX_STANDARD 11)

include_directories(SYSTEM)

find_package(spdlog REQUIRED)

add_executable(main main.cpp
        fm/EventLoop.cpp fm/EventLoop.h fm/noncopyable.h fm/Until.cpp fm/Until.h
        fm/Channel.cpp
        fm/Poller.cpp
        fm/TimerQueue.cpp
        fm/Timer.cpp
        fm/InetAddress.cpp
        fm/Socket.cpp
        fm/Acceptor.cpp
        fm/TcpServer.cpp
        fm/TcpConnection.cpp
        fm/Buffer.cpp
        fm/IgnoreSigPipe.cpp fm/IgnoreSigPipe.h fm/EventLoopThread.cpp fm/EventLoopThread.h fm/EventLoopThreadPool.cpp fm/EventLoopThreadPool.h fm/Connector.cpp fm/Connector.h fm/TcpClient.cpp fm/TcpClient.h fm/HttpServer.cpp fm/HttpServer.h)


add_executable(client client.cpp
        fm/EventLoop.cpp fm/EventLoop.h fm/noncopyable.h fm/Until.cpp fm/Until.h
        fm/Channel.cpp
        fm/Poller.cpp
        fm/TimerQueue.cpp
        fm/Timer.cpp
        fm/InetAddress.cpp
        fm/Socket.cpp
        fm/Acceptor.cpp
        fm/TcpServer.cpp
        fm/TcpConnection.cpp
        fm/Buffer.cpp
        fm/IgnoreSigPipe.cpp fm/IgnoreSigPipe.h fm/EventLoopThread.cpp fm/EventLoopThread.h fm/EventLoopThreadPool.cpp fm/EventLoopThreadPool.h fm/Connector.cpp fm/Connector.h fm/TcpClient.cpp fm/TcpClient.h)

target_link_libraries(main PRIVATE spdlog::spdlog)

target_link_libraries(client PRIVATE spdlog::spdlog)


add_executable(http http.cpp
        fm/EventLoop.cpp fm/EventLoop.h fm/noncopyable.h fm/Until.cpp fm/Until.h
        fm/Channel.cpp
        fm/Poller.cpp
        fm/TimerQueue.cpp
        fm/Timer.cpp
        fm/InetAddress.cpp
        fm/Socket.cpp
        fm/Acceptor.cpp
        fm/TcpServer.cpp
        fm/TcpConnection.cpp
        fm/Buffer.cpp
        fm/IgnoreSigPipe.cpp fm/IgnoreSigPipe.h fm/EventLoopThread.cpp fm/EventLoopThread.h fm/EventLoopThreadPool.cpp fm/EventLoopThreadPool.h fm/Connector.cpp fm/Connector.h fm/TcpClient.cpp fm/TcpClient.h)

target_link_libraries(http PRIVATE spdlog::spdlog)
