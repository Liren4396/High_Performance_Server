#include "include/BufferPool.h"
#include <algorithm>

// thread_local缓存：每个线程有自己的Buffer缓存，减少锁竞争
thread_local std::vector<std::shared_ptr<Buffer>> thread_local_cache;

// 获取单例对象池
BufferPool& BufferPool::getInstance() {
    static BufferPool instance;
    return instance;
}

// 获取一个 Buffer 实例（优化：优先从thread_local缓存获取）
std::shared_ptr<Buffer> BufferPool::getBuffer() {
    // 优先从thread_local缓存获取（无锁）
    if (!thread_local_cache.empty()) {
        auto buf = thread_local_cache.back();
        thread_local_cache.pop_back();
        return buf;
    }
    
    // thread_local缓存为空，从全局池获取（需要加锁）
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (!pool.empty()) {
            auto buf = pool.back();
            pool.pop_back();
            return buf;
        }
    }
    
    // 全局池也为空，创建新的
    return std::make_shared<Buffer>();
}

// 归还 Buffer 实例（优化：优先放入thread_local缓存）
void BufferPool::returnBuffer(std::shared_ptr<Buffer> buf) {
    if (!buf) return;
    
    buf->clear();  // 清空数据
    
    // 优先放入thread_local缓存（无锁）
    if (thread_local_cache.size() < THREAD_LOCAL_CACHE_SIZE * 2) {
        thread_local_cache.push_back(std::move(buf));
    } else {
        // thread_local缓存已满，回收到全局池（需要加锁）
        std::lock_guard<std::mutex> lock(mtx);
        pool.push_back(std::move(buf));
    }
}