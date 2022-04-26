//
// Created by wyatt on 2022/4/21.
//

#ifndef SOCKETFRAMEWORK_SOCKET_H
#define SOCKETFRAMEWORK_SOCKET_H

#include "InetAddress.h"
#include <string>

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
};


#endif //SOCKETFRAMEWORK_SOCKET_H
