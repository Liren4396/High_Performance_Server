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
    // 获取一个 Buffer 实例（优化：使用thread_local缓存减少锁竞争）
    std::shared_ptr<Buffer> getBuffer();
    // 归还 Buffer 实例（优化：优先放入thread_local缓存）
    void returnBuffer(std::shared_ptr<Buffer> buf);

private:
    BufferPool() = default; // 禁止外部构造
    ~BufferPool() = default;

    // 全局对象池（需要加锁）
    std::vector<std::shared_ptr<Buffer>> pool; 
    std::mutex mtx;
    
    // thread_local缓存大小（每个线程缓存几个Buffer）
    static constexpr size_t THREAD_LOCAL_CACHE_SIZE = 4;
};

#endif // BUFFER_POOL_H
