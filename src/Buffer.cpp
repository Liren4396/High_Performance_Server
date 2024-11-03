#include "Buffer.h"
#include <string.h>
#include <iostream>

Buffer::Buffer() {}

Buffer::~Buffer() {}

void Buffer::append(const char* _str, int _size) {
    for (int i = 0; i < _size; ++i) {
        if (_str[i] == '\0') break;
        buf.push_back(_str[i]);
    }
}
void Buffer::SetBuf(std::string buffer) {
    buf = buffer;
}
ssize_t Buffer::size() {
    return buf.size();
}

const char *Buffer::ToStr() { return buf.c_str(); }

const char* Buffer::c_str() {
    return buf.c_str();
}

std::string Buffer::getBuffer() {
    return buf;
}

void Buffer::clear() {
    buf.clear();
}

void Buffer::getline() {
    buf.clear();
    std::getline(std::cin, buf);
}