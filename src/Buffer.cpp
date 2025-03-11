// Buffer.cpp
#include "include/Buffer.h"
#include <string.h>
#include <iostream>
#include <mutex>
#include <sstream>

Buffer::Buffer() {}

Buffer::~Buffer() {}

void Buffer::append(const char* _str, int _size) {
    // 忽略特殊起始字符的数据
    if (_str[0] == 25 || _str[0] == 23) return;
    std::lock_guard<std::mutex> lock(mtx);
    for (int i = 0; i < _size; ++i) {
        if (_str[i] == '\0') break;
        backBuffer.push_back(_str[i]);
    }
}

ssize_t Buffer::size() {
    std::lock_guard<std::mutex> lock(mtx);
    return backBuffer.size();
}

const char* Buffer::c_str() {
    std::lock_guard<std::mutex> lock(mtx);
    return backBuffer.c_str();
}

void Buffer::clear() {
    std::lock_guard<std::mutex> lock(mtx);
    backBuffer.clear();
}

void Buffer::getline() {
    std::lock_guard<std::mutex> lock(mtx);
    backBuffer.clear();
    std::getline(std::cin, backBuffer);
}

std::string Buffer::getBuffer() {
    std::lock_guard<std::mutex> lock(mtx);
    return backBuffer;
}

void Buffer::SetBuf(std::string buffer) {
    std::lock_guard<std::mutex> lock(mtx);
    backBuffer = buffer;
}

const char* Buffer::ToStr() {
    std::lock_guard<std::mutex> lock(mtx);
    return backBuffer.c_str();
}

void Buffer::setName(std::string name) {
    std::lock_guard<std::mutex> lock(mtx);
    _name = name;
}

std::string Buffer::getName() {
    std::lock_guard<std::mutex> lock(mtx);
    return _name;
}

std::string Buffer::swapBuffers() {
    std::lock_guard<std::mutex> lock(mtx);
    // 将接收缓冲区和处理缓冲区交换，后续处理逻辑直接操作 frontBuffer，
    // 同时清空接收缓冲区以便继续接收数据
    frontBuffer.swap(backBuffer);
    backBuffer.clear();
    return frontBuffer;
}
