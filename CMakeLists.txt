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
        )

target_link_libraries(main PRIVATE spdlog::spdlog)


