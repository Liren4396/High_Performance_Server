#ifndef BUFFER_POOL_H
#define BUFFER_POOL_H
#include "Buffer.h"
#include <vector>
#include <memory>
#include <mutex>

class BufferPool {
public:
    // 获取单例对象池
    static BufferPool& getInstance();
    // 获取一个 Buffer 实例
    std::shared_ptr<Buffer> getBuffer();
    // 归还 Buffer 实例
    void returnBuffer(std::shared_ptr<Buffer> buf);

private:
    BufferPool() = default; // 禁止外部构造
    ~BufferPool() = default;

    std::vector<std::shared_ptr<Buffer>> pool; // 维护 Buffer 对象
    std::mutex mtx; // 线程安全
};

#endif // BUFFER_POOL_H
