//
// Created by wyatt on 2022/4/21.
//

#include "InetAddress.h"
#include "Socket.h"
#include <iostream>
#include <arpa/inet.h>
#include <cstring>

static const in_addr_t kInaddrAny = INADDR_ANY;
static const in_addr_t kInaddrLoopback = INADDR_LOOPBACK;


InetAddress::InetAddress(uint16_t port, bool loopBackOnly, bool ipv6) {
    bzero(&addr, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    in_addr_t ip = loopBackOnly ? kInaddrLoopback : kInaddrAny;
    addr.sin_addr.s_addr = htobe32(ip);
    addr.sin_port = htons(port);
    printf("s_addr = %#x ,port : %#x\r\n", addr.sin_addr.s_addr, addr.sin_port);
}

sockaddr *InetAddress::getSocketAddress() const {
    return (struct sockaddr *) (&addr);
}

int InetAddress::getStructSize() const {
    return sizeof(addr);
}

std::string InetAddress::toIp() const {
    char buf[64];
    Socket::toIp(getSocketAddress(), buf, sizeof(buf));
    return buf;
}

