//
// Created by wyatt on 2022/4/21.
//

#include "Buffer.h"

int Buffer::read(int fd) {
//    WYATT_LOG_ROOT_DEBUG() << "read fd: " << fd;
    char extraBuff[65535];
    struct iovec vec[2];
    vec[0].iov_base = begin() + writeIndex;
    vec[0].iov_len = getWritable();
    vec[1].iov_base = extraBuff;
    vec[1].iov_len = sizeof(std::exception);
//    WYATT_LOG_ROOT_DEBUG() << "getWritable:" << getWritable();
    int n = readv(fd, vec, 2);
    if (n < 0) {
//        WYATT_LOG_ROOT_DEBUG() << "buffer read error:" << errno;
    } else if (n <= getWritable()) {
        writeIndex += (int) n;
    } else {
        writeIndex = (int) buf.size();
        append(extraBuff, (int) n - getWritable());
    }
    return n;
}

void Buffer::append(char *data, int n) {
    if (getWritable() >= n)
    {
        // 可以直接写入

    }else if ((readIndex - preAppendIndex) + (buf.size() - writeIndex) <= n) {
        // 空间够用，只要把久数据移动到前面，然后再添加进来
        std::copy(begin() + readIndex, begin() + writeIndex, begin() + preAppendIndex);
        writeIndex = preAppendIndex + getReadIndex();
        readIndex = preAppendIndex;
    } else {
        // 空间不够用
        buf.resize(writeIndex + n);
    }
    std::copy(data, data + n, begin() + writeIndex);
    writeIndex += n;
}

void Buffer::retrieve(int len) {
    if (len <= getReadable()) readIndex += len;
    if (readIndex == writeIndex) {
        writeIndex = readIndex = preAppendIndex;
    }
}