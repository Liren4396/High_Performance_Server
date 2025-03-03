# High Performance Chat Server 🚀

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
![C++](https://img.shields.io/badge/C++-17-%2300599C?logo=c%2B%2B)

一个基于C++17实现的高性能聊天服务器，采用Reactor多线程模型，支持非阻塞IO和MySQL数据持久化。

## 项目亮点 ✨

- **高性能架构**：基于Reactor多线程模型，QPS可达1k+
- **智能IO管理**：epoll ET模式 + 非阻塞IO + 事件驱动
- **数据持久化**：集成MySQL8.0，完整记录用户行为日志
- **安全稳定**：连接资源自动回收，异常处理机制完善
- **跨平台**：兼容Linux/Unix系统（已在CentOS 7.6验证）

### 核心组件
- **网络模型**：Reactor + ThreadPool
- **事件驱动**：epoll ET模式
- **连接管理**：自定义Buffer + 智能指针
- **数据存储**：MySQL连接池
- **异常处理**：统一错误码机制

## 功能特性 📦

### 网络层
- 多线程IO处理
- 非阻塞Socket通信
- 自动心跳检测
- 连接超时管理

### 业务层
- 实时群聊广播
- 用户上下线通知
- 消息格式化传输（自定义消息头）
- 连接资源自动回收

### 数据层
- 用户登录记录（IP/时间戳）
- 聊天历史存储
- 在线用户管理
- 数据自动归档

## 搭建过程 🛠️

1. 建立 C/S 通信架构  
2. 建立一对多的服务器与客户端连接（C++ 风格）  
3. 引入缓冲区  
4. 引入 Reactor 单线程模式  
5. 引入线程池  
6. 引入 Reactor 多线程模式  
7. 客户端修改为非阻塞，引入 epoll  
8. 服务器增加聊天室功能，任何连入的客户端都能收到对方的消息  
9. 引入 MySQL，数据永久保存并记录登录用户相关数据  

## 性能优化 🔧

- 使用对象池管理连接资源  
- 双缓冲技术减少锁竞争  
- 零拷贝数据传输  
- 定时器轮处理超时连接  

## 扩展计划 💡

- WebSocket 协议支持  
- 分布式集群部署  
- Redis 缓存集成  
- 消息加密传输  

## 快速开始 🚀

### 环境要求
- GCC 9.0+
- CMake 3.12+
- MySQL 8.0+
- Linux Kernel 3.10+

### 构建步骤
```bash
git clone https://github.com/yourusername/High_Performance_Server.git
cd High_Performance_Server
mkdir build && cd build
cmake ..
make -j$(nproc)
