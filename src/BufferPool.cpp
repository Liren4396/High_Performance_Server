#include "include/BufferPool.h"

// 获取单例对象池
BufferPool& BufferPool::getInstance() {
    static BufferPool instance;
    return instance;
}

// 获取一个 Buffer 实例
std::shared_ptr<Buffer> BufferPool::getBuffer() {
    std::lock_guard<std::mutex> lock(mtx);
    if (!pool.empty()) {
        auto buf = pool.back();
        pool.pop_back();
        return buf;
    }
    return std::make_shared<Buffer>(); // 池子空了就创建新的
}

// 归还 Buffer 实例
void BufferPool::returnBuffer(std::shared_ptr<Buffer> buf) {
    std::lock_guard<std::mutex> lock(mtx);
    buf->clear();  // 清空数据
    pool.push_back(std::move(buf)); // 放回池子
}