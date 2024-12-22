#include "include/Buffer.h"
#include <string.h>
#include <iostream>

Buffer::Buffer() {}

Buffer::~Buffer() {}

void Buffer::setName(std::string name) {
    _name = name;
}

std::string Buffer::getName() {
    return _name;
}

void Buffer::append(const char* _str, int _size) {
    if (_str[0] == 25 || _str[0] == 23) return;
    for (int i = 0; i < _size; ++i) {
        std::cout << (int)_str[i] << std::endl;
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