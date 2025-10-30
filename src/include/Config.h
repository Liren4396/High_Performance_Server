// Config.h
#include <atomic>
#define PORT 8080
#define ADDRESS "127.0.0.1"
extern std::atomic<int> g_qps_counter;