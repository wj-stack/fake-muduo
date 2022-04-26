//
// Created by wyatt on 2022/4/21.
//

#ifndef SOCKETFRAMEWORK_BUFFER_H
#define SOCKETFRAMEWORK_BUFFER_H

#include <vector>
#include <sys/socket.h>
#include <sys/uio.h>

class Buffer {

private:
    std::vector<char> buf;
    int writeIndex;
    int preAppendIndex;
    int readIndex;

public:
    Buffer(int preAppend = 8, int size = 1024) : buf(size), preAppendIndex(preAppend), readIndex(preAppend),
                                                 writeIndex(preAppend) {}

    char *begin() { return buf.data(); }

    int getWritable() const { return buf.size() - writeIndex; }

    int getReadable() const { return writeIndex - readIndex; }

    void retrieve(int len); // 给用户调用，取回数据后要修改readIndex

    int read(int fd);

    void append(char *data, int n);

    size_t getCapacity()
    {
        return buf.capacity();
    }

    int getReadIndex() const {
        return readIndex;
    }

    void setReadIndex(int i) {
        Buffer::readIndex = i;
    }

    int getWriteIndex() const {
        return writeIndex;
    }

    void setWriteIndex(int i) {
        Buffer::writeIndex = i;
    }


};


#endif //SOCKETFRAMEWORK_BUFFER_H
