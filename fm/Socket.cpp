//
// Created by wyatt on 2022/4/21.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <csignal>
#include "Socket.h"
#include "spdlog/spdlog.h"

int Socket::createSocket() {
    int fd = ::socket(AF_INET,
                      SOCK_STREAM | SOCK_NONBLOCK, 0);
    if (fd < 0) {
        SPDLOG_INFO("create socket error");
    }
    return fd;
}

bool Socket::bind(int fd, const InetAddress &listenAddress) {

    // 设置端口复用
    int on = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    if (::bind(fd, listenAddress.getSocketAddress(), listenAddress.getStructSize()) == -1) {
        spdlog::info("bind socket error: {}", strerror(errno));
        return false;
    }
    return true;
}

bool Socket::listen(int fd, int n) {

    if (::listen(fd, n) == -1) {
        spdlog::info("listen socket error: {}", strerror(errno));
        return false;
    }
    return true;
}

int Socket::accept(int fd, InetAddress &inetAddress) {
    struct sockaddr_in6 addr{};
    bzero(&addr, sizeof addr);
    int conn = accept(fd, &addr);
    inetAddress.setSockAddrInet6(addr);
    return conn;
}

int Socket::accept(int sockfd, struct sockaddr_in6 *addr) {
    socklen_t addrlen = static_cast<socklen_t>(sizeof *addr);
#if VALGRIND || defined (NO_ACCEPT4)
    int connfd = ::accept(sockfd, sockaddr_cast(addr), &addrlen);
  setNonBlockAndCloseOnExec(connfd);
#else
    int connfd = ::accept4(sockfd, (struct sockaddr *) (addr),
                           &addrlen, SOCK_NONBLOCK | SOCK_CLOEXEC);
#endif
    if (connfd < 0) {
        int savedErrno = errno;
        spdlog::info("accept error: {}", strerror(errno));
        switch (savedErrno) {
            case EAGAIN:
            case ECONNABORTED:
            case EINTR:
            case EPROTO: // ???
            case EPERM:
            case EMFILE: // per-process lmit of open file desctiptor ???
                // expected errors
                errno = savedErrno;
                break;
            case EBADF:
            case EFAULT:
            case EINVAL:
            case ENFILE:
            case ENOBUFS:
            case ENOMEM:
            case ENOTSOCK:
            case EOPNOTSUPP:
                // unexpected errors
                spdlog::info("unexpected errors: {}", strerror(errno));
                break;
            default:
                spdlog::info("accept error: {}", strerror(errno));
                break;
        }
    }
    return connfd;
}

void Socket::toIp(const struct sockaddr *addr, char *buf, int size) {

    if (addr->sa_family == AF_INET) {
        const struct sockaddr_in *addr4 = (struct sockaddr_in *) (addr);
//        WYATT_LOG_ROOT_DEBUG() << "addr4" << addr4;
        ::inet_ntop(AF_INET, &addr4->sin_addr, buf, static_cast<socklen_t>(size));
    } else if (addr->sa_family == AF_INET6) {
        assert(size >= INET6_ADDRSTRLEN);
        const struct sockaddr_in6 *addr6 = (struct sockaddr_in6 *) (addr);
        ::inet_ntop(AF_INET6, &addr6->sin6_addr, buf, static_cast<socklen_t>(size));
    }
}

void Socket::close(int fd) {
    ::close(fd);
}
