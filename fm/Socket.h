//
// Created by wyatt on 2022/4/21.
//

#ifndef SOCKETFRAMEWORK_SOCKET_H
#define SOCKETFRAMEWORK_SOCKET_H

#include "InetAddress.h"
#include <string>
#include <arpa/inet.h>
#include <byteswap.h>
#include "spdlog/spdlog.h"
class Socket {
public:
    static bool bind(int fd, const InetAddress &listenAddress);

    Socket() = default;

    static int createSocket();

    static bool listen(int fd, int n);

    static int accept(int fd, InetAddress &inetAddress);

    static void close(int fd);

    static void toIp(const sockaddr *addr, char *buf, int size);

    static int accept(int sockfd, sockaddr_in6 *addr);


    static const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr)
    {
        return static_cast<const struct sockaddr*>((const void*)(addr));
    }

    static void fromIpPort(const char* ip, uint16_t port,
                             struct sockaddr_in* addr)
    {
        addr->sin_family = AF_INET;
        addr->sin_port = bswap_16(port);
        if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
        {
            spdlog::error("sockets::fromIpPort");
        }
    }


    static int getSocketError(int sockfd)
    {
        int optval;
        socklen_t optlen = static_cast<socklen_t>(sizeof optval);

        if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
        {
            return errno;
        }
        else
        {
            return optval;
        }
    }


    static struct sockaddr_in6 getLocalAddr(int sockfd)
    {
        struct sockaddr_in6 localaddr;
        bzero(&localaddr, sizeof localaddr);
        socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
        sockaddr * p = const_cast<sockaddr *>(sockaddr_cast(&localaddr));
        if (::getsockname(sockfd, p, &addrlen) < 0)
        {
            spdlog::error("sockets::getLocalAddr");
        }
        return localaddr;
    }

    static struct sockaddr_in6 getPeerAddr(int sockfd)
    {
        struct sockaddr_in6 peeraddr;
        bzero(&peeraddr, sizeof peeraddr);
        socklen_t addrlen = static_cast<socklen_t>(sizeof peeraddr);
        sockaddr * p = const_cast<sockaddr *>(sockaddr_cast(&peeraddr));
        if (::getpeername(sockfd, p, &addrlen) < 0)
        {
            spdlog::error("sockets::getPeerAddr");
        }
        return peeraddr;
    }
    static bool isSelfConnect(int sockfd)
    {
        struct sockaddr_in6 localaddr = getLocalAddr(sockfd);
        struct sockaddr_in6 peeraddr = getPeerAddr(sockfd);
        if (localaddr.sin6_family == AF_INET)
        {
            const struct sockaddr_in* laddr4 = reinterpret_cast<struct sockaddr_in*>(&localaddr);
            const struct sockaddr_in* raddr4 = reinterpret_cast<struct sockaddr_in*>(&peeraddr);
            return laddr4->sin_port == raddr4->sin_port
                   && laddr4->sin_addr.s_addr == raddr4->sin_addr.s_addr;
        }
        else if (localaddr.sin6_family == AF_INET6)
        {
            return localaddr.sin6_port == peeraddr.sin6_port
                   && memcmp(&localaddr.sin6_addr, &peeraddr.sin6_addr, sizeof localaddr.sin6_addr) == 0;
        }
        else
        {
            return false;
        }
    }

    static void shutdownWrite(int sockfd)
    {
        if (::shutdown(sockfd, SHUT_WR) < 0)
        {
            SPDLOG_INFO("sockets::shutdownWrite");
        }
    }



};


#endif //SOCKETFRAMEWORK_SOCKET_H
