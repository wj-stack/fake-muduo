cmake_minimum_required(VERSION 3.22)
project(fm)

set(CMAKE_CXX_STANDARD 11)

include_directories(SYSTEM)

add_definitions(-DANDROID_PLATFORM=android-21)

message(STATUE "info:" ${ANDROID_ABI})

set(ANDROID_PLATFORM android-21)
set(ANDROID_PLATFORM_LEVEL 21)

set(FETCHCONTENT_UPDATES_DISCONNECTED_Catch2 ON) # 防止一直加载spdlog

Include(FetchContent)

# spdlog
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.9.2
)
FetchContent_MakeAvailable(spdlog)

if(NOT TARGET spdlog)
    # Stand-alone build
    find_package(spdlog REQUIRED)
endif()


## jsoncpp
#FetchContent_Declare(
#        jsoncpp
#        GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
#        GIT_TAG 1.9.3
#)
#FetchContent_MakeAvailable(jsoncpp)
#if(NOT TARGET jsoncpp)
#    # Stand-alone build
#    find_package(jsoncpp REQUIRED)
#endif()
include_directories(jsoncpp)
add_subdirectory(jsoncpp)

#message(STATUE "BUILD_STATIC_LIBS:" ${BUILD_STATIC_LIBS})

add_executable(main main.cpp
        fm/EventLoop.cpp
        fm/EventLoop.h
        fm/noncopyable.h
        fm/Until.cpp
        fm/Until.h
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
        fm/IgnoreSigPipe.cpp
        fm/IgnoreSigPipe.h
        fm/EventLoopThread.cpp
        fm/EventLoopThread.h
        fm/EventLoopThreadPool.cpp
        fm/EventLoopThreadPool.h
        fm/Connector.cpp
        fm/Connector.h
        fm/TcpClient.cpp
        fm/TcpClient.h
        fm/HttpServer.cpp
        fm/HttpServer.h
        )

file(GLOB_RECURSE threadPool ThreadPool/*.cpp)
file(GLOB_RECURSE memoryTools MemoryTools/*.cpp)
file(GLOB_RECURSE lexicalCast LexicalCast/*.cpp)

add_library(threadPool STATIC ${threadPool})
add_library(memoryTools STATIC ${memoryTools})
add_library(lexicalCast STATIC ${lexicalCast})

target_link_libraries(main PRIVATE spdlog::spdlog jsoncpp_object threadPool memoryTools lexicalCast)

