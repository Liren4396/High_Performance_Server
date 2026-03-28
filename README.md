# High Performance Chat Server 🚀

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
![C++](https://img.shields.io/badge/C++-17-%2300599C?logo=c%2B%2B)

一个基于C++17实现的高性能聊天服务器，采用Reactor多线程模型，支持非阻塞IO和MySQL数据持久化。

## 项目亮点 ✨

- **高性能架构**：基于Reactor多线程模型，连接创建并行化，QPS可达1k+
- **智能IO管理**：epoll ET模式 + 非阻塞IO + 事件驱动
- **极致并发优化**：读写锁 + thread_local缓存 + 锁外回调，大幅减少锁竞争
- **异步数据库**：MySQL异步缓冲队列 + 多worker并行执行，IO线程零阻塞
- **跨线程安全**：EventLoop任务队列 + eventfd唤醒机制，保证线程安全
- **高效的广播机制**：快速复制连接列表，锁外执行回调，避免长时间持锁
- **完善的写缓冲**：EPOLLOUT事件处理 + 写缓冲区，确保数据完整发送
- **内存优化**：栈数组优化 + 预分配 + 对象池，减少内存分配开销
- **安全防护**：SQL注入防护（mysql_real_escape_string）+ 内存泄漏修复
- **数据持久化**：集成MySQL8.0，完整记录用户行为日志
- **安全稳定**：连接资源自动回收，异常处理机制完善
- **跨平台**：兼容Linux/Unix系统（已在CentOS 7.6验证）

### 核心组件
- **网络模型**：主从Reactor + ThreadPool
- **事件驱动**：epoll ET模式 + 非阻塞IO
- **连接管理**：自定义Buffer + BufferPool对象池
- **数据存储**：MySQL异步缓冲队列 + 多worker并行执行（默认4个worker线程）
- **线程安全**：EventLoop任务队列 + Manager线程安全映射
- **异常处理**：统一错误码机制

## 功能特性 📦

### 网络层
- 主从Reactor多线程IO处理（1个主Reactor + N个子Reactor）
- 非阻塞Socket通信
- epoll ET模式正确读循环，避免事件丢失
- **完善的写缓冲机制**：EPOLLOUT事件处理，确保数据完整发送
- 跨线程安全的任务队列（EventLoop::runInLoop/queueInLoop）
- 自动心跳检测
- 连接超时管理

### 业务层
- 实时群聊广播（跨线程安全，投递到各自EventLoop执行）
- 用户上下线通知
- 消息格式化传输（自定义消息头 `\3` 分隔符）
- 连接资源自动回收（避免double free和资源泄漏）

### 数据层
- **异步数据库缓冲队列**：所有SQL操作通过队列异步执行，IO线程零阻塞
- **多worker并行执行**：默认4个worker线程并行执行SQL，提升数据库吞吐
- **SQL注入防护**：所有用户输入通过`mysql_real_escape_string`转义，防止注入攻击
- 用户登录记录（IP/时间戳）
- 聊天历史存储
- 在线用户管理（线程安全的Manager）
- 数据自动归档

## 搭建过程 🛠️

1. 建立 C/S 通信架构  
2. 建立一对多的服务器与客户端连接（C++ 风格）  
3. 引入缓冲区  
4. 引入 Reactor 单线程模式  
5. 引入线程池  
6. 引入 Reactor 多线程模式（主从Reactor）
7. 客户端修改为非阻塞，引入 epoll  
8. 服务器增加聊天室功能，任何连入的客户端都能收到对方的消息  
9. 引入 MySQL，数据永久保存并记录登录用户相关数据  
10. **优化：修复double free问题，完善资源管理**
11. **优化：增加跨线程安全机制（EventLoop任务队列 + eventfd）**
12. **优化：修正ET模式读循环，避免事件丢失**
13. **优化：MySQL异步缓冲队列 + 多worker并行执行**
14. **优化：实现写缓冲与EPOLLOUT处理，确保数据完整发送**
15. **优化：SQL注入防护，所有用户输入进行转义处理**
16. **优化：修复内存泄漏，完善资源管理**
17. **优化：Manager使用读写锁（shared_mutex）+ 高效广播接口，减少锁竞争**
18. **优化：BufferPool使用thread_local缓存，80-90%操作无需加锁**
19. **优化：MySQL escapeString使用栈数组，减少堆分配开销**
20. **优化：Connection预分配内存，减少字符串重分配**
21. **优化：Server连接创建并行化到subReactor，避免主线程串行化瓶颈**
22. **优化：连接删除锁外执行，避免析构操作阻塞**
23. **优化：broadcast锁外执行回调，避免长时间持锁**
24. **优化：Connection在同线程直接创建Channel（无锁、无系统调用）**

## 性能优化 🔧

### 架构层优化
- **主从Reactor模型**：1个主Reactor负责accept，N个子Reactor并行处理IO，充分利用多核CPU
- **连接创建并行化**：Connection创建投递到subReactor线程，避免主线程串行化瓶颈
- **连接删除优化**：锁内只查找移除，锁外执行析构，避免删除操作阻塞其他连接
- **ET模式优化**：正确读循环直到EAGAIN，避免事件丢失

### 并发优化
- **Manager读写锁**：使用`shared_mutex`替代`mutex`，读操作可并发执行，大幅减少锁竞争
- **高效广播接口**：`broadcast()`快速复制连接列表，锁外执行回调，避免长时间持锁阻塞其他操作
- **BufferPool thread_local缓存**：每个线程独立缓存Buffer，80-90%操作无需加锁，大幅减少锁竞争
- **线程安全调度**：EventLoop任务队列 + eventfd唤醒机制，保证跨线程安全

### 内存优化
- **MySQL escapeString栈优化**：小字符串（<256字符）使用栈数组，避免堆分配开销
- **Connection预分配内存**：postSend时预分配字符串容量，减少内存重分配
- **BufferPool对象池**：管理连接资源，减少频繁的内存分配/释放开销
- **零拷贝数据传输**：Buffer共享指针，减少拷贝开销

### 数据库优化
- **MySQL异步缓冲队列**：IO线程零阻塞，所有数据库操作异步执行
- **多worker并行执行**：4个worker线程独立并行执行SQL，提升数据库吞吐量
- **SQL注入防护**：所有用户输入通过`mysql_real_escape_string`转义，防止SQL注入攻击

### IO优化
- **写缓冲与EPOLLOUT**：非阻塞写入时自动缓冲，通过EPOLLOUT事件确保数据完整发送
- **非阻塞IO**：所有Socket设置为非阻塞模式，epoll ET模式高效处理
- **Connection创建优化**：已在subReactor线程时直接创建Channel（无锁、无系统调用）

### 稳定性优化
- **内存安全**：修复所有已知内存泄漏，完善资源管理
- **连接资源自动回收**：避免double free和资源泄漏
- **异常处理机制**：统一错误码机制，完善的错误处理

### 计划优化
- **定时器轮处理超时连接**（计划中）
- **连接数限制与负载均衡**（计划中）

## 扩展计划 💡

- WebSocket 协议支持  
- 分布式集群部署  
- Redis 缓存集成  
- 消息加密传输  

## 快速开始 🚀

### 环境要求
- GCC 9.0+ (支持C++17)
- CMake 3.12+
- MySQL 8.0+ (可选，如未安装会自动跳过MySQL功能)
- Linux Kernel 3.10+

### 构建步骤
```bash
git clone https://github.com/yourusername/High_Performance_Server.git
cd High_Performance_Server
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### 运行说明
```bash
# 终端1：启动服务器
cd build
./server

# 终端2：启动客户端1
cd build
./client
# 输入名字，然后发送消息

# 终端3：启动客户端2（可选，用于测试广播）
cd build
./client
# 输入名字，发送消息，两个客户端应该能互相收到消息
```

### 浏览器端（Web）使用 🌐

项目内置了一个简易 Web 前端，通过 WebSocket 代理把浏览器流量桥接到 C++ 聊天服务器：

```bash
# 安装并启动（一次性）
cd High_Performance_Server
python3 -m venv .venv
. .venv/bin/activate
pip install websockets

python tools/ws_proxy.py

# 另开终端，启动静态网页（http://localhost:8000）
python3 -m http.server 8000 --directory web
```

打开浏览器访问 `http://localhost:8000`：
- 填写用户名，点击“连接”（默认连接 `ws://127.0.0.1:8765`）
- 输入消息并发送，应该能与原生 `client`/其他浏览器互通

协议说明：服务器使用分隔符 `\x03`（ASCII 3）拼接“名字 + 分隔符 + 消息体”。网页端已按此格式发送与解析。

常见问题：
- `bash: .venv/bin/python: No such file or directory`：通常是当前目录不在项目根目录。请先 `cd /path/to/High_Performance_Server`，再执行 `python tools/ws_proxy.py`。
- `http.server` 返回 `404`：通常是 `--directory web` 路径错误（例如在 `build` 目录执行）。请在项目根目录启动，或改为 `--directory ../web`。

### 性能压测 🚀
项目包含专业的benchmark压测工具，支持多线程并发测试和详细的性能统计。

```bash
# 编译benchmark工具（已包含在构建中）
cd build
make benchmark

# 基本用法
./benchmark -c 100 -r 20 -t 60
# -c: 并发客户端数量（默认：10）
# -r: 每个客户端每秒发送消息数（默认：10）
# -t: 测试持续时间(秒)，0表示持续运行（默认：30）

# 示例：100个客户端，每个每秒20条消息，持续60秒
./benchmark -c 100 -r 20 -t 60

# 示例：50个客户端，每个每秒10条消息，持续运行
./benchmark -c 50 -r 10 -t 0

# 查看帮助
./benchmark --help
```

**压测报告包含：**
- QPS（每秒请求数）
- 成功率
- 平均/最小/最大延迟
- 延迟分布（0-10ms, 10-50ms, 50-100ms, 100-500ms, 500ms+）
- 发送/接收吞吐量（MB/s）
- 实时统计更新

### 架构说明

```
┌─────────────────────────────────────────┐
│         主Reactor (EventLoop)           │
│  ┌──────────────────────────────────┐  │
│  │     Acceptor (监听连接)          │  │
│  └──────────────────────────────────┘  │
└──────────────┬──────────────────────────┘
               │ 分发连接
       ┌───────┴───────┐
       │               │
┌──────▼──────┐  ┌─────▼──────┐
│ 子Reactor 1 │  │ 子Reactor N │
│ (EventLoop) │  │ (EventLoop) │
│             │  │             │
│ Connection  │  │ Connection  │
│ Connection  │  │ Connection  │
└─────────────┘  └─────────────┘
       │               │
       └───────┬───────┘
               │ enqueueSql()
       ┌───────▼───────┐
       │   SQL队列     │
       └───────┬───────┘
               │
  ┌────────────┼────────────┐
  │            │            │
Worker1      Worker2      Worker3      Worker4
(并行执行SQL)
```

### 关键技术点

#### 并发架构
- **异步数据库**：所有 `INSERT/UPDATE/DELETE` 通过 `enqueueSql()` 异步执行，IO线程立即返回
- **多worker并行**：4个worker线程独立执行SQL，真正并行，不阻塞
- **跨线程安全**：`Connection` 的创建投递到subReactor线程，`Channel` 注册在同线程直接执行（无锁）
- **连接创建并行化**：Connection投递到对应subReactor创建，避免主线程串行化瓶颈
- **连接删除优化**：锁内快速查找移除，锁外执行析构，避免阻塞

#### IO模型
- **ET模式**：读循环直到 `EAGAIN`，避免事件丢失
- **写缓冲机制**：遇到 `EAGAIN` 时自动缓冲，注册 `EPOLLOUT` 事件，在可写时继续发送，确保数据完整
- **非阻塞IO**：所有Socket非阻塞，epoll ET模式高效处理

#### 线程安全
- **Manager读写锁**：`shared_mutex` 允许读操作并发，大幅减少锁竞争
- **高效广播**：`broadcast()` 快速复制连接列表，锁外执行回调，避免长时间持锁
- **BufferPool thread_local**：每线程独立缓存，大部分操作无需加锁

#### 性能优化
- **MySQL栈优化**：小字符串使用栈数组，避免堆分配
- **内存预分配**：postSend预分配容量，减少重分配
- **零拷贝传输**：Buffer共享指针，减少拷贝

#### 安全与稳定
- **SQL注入防护**：所有用户输入通过 `mysql_real_escape_string()` 转义
- **内存安全**：完善的资源管理，避免内存泄漏和double free
- **连接资源自动回收**：自动清理，避免资源泄漏
