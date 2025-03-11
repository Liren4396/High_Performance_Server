//Buffer.h
#ifndef BUFFER_H
#define BUFFER_H

#include <string>
#include <mutex>

class Buffer {
public:
    Buffer();
    ~Buffer();

    // 向接收缓冲区（backBuffer）追加数据
    void append(const char* _str, int _size);

    // 获取接收缓冲区的大小
    ssize_t size();

    // 返回接收缓冲区的 C 字符串指针
    const char* c_str();

    // 清空接收缓冲区
    void clear();

    // 从标准输入读取一行数据到接收缓冲区
    void getline();

    // 获取接收缓冲区的内容（副本）
    std::string getBuffer();

    // 设置接收缓冲区的内容
    void SetBuf(std::string buffer);

    // 返回接收缓冲区的 C 字符串指针
    const char* ToStr();

    // 设置和获取名称
    void setName(std::string name);
    std::string getName();

    // 双缓冲核心操作：交换接收缓冲区和处理缓冲区，
    // 返回交换后用于处理的缓冲区内容，并清空接收缓冲区
    std::string swapBuffers();

private:
    std::string _name;
    std::string frontBuffer;  // 用于后续数据处理的缓冲区
    std::string backBuffer;   // 数据接收缓冲区
    std::mutex mtx;           // 保护缓冲区数据的互斥量
};

#endif // BUFFER_H
