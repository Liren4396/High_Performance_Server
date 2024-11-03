
#include <string>
class Buffer {
public:
    Buffer();
    ~Buffer();
    void append(const char* _str, int _size);
    ssize_t size();
    const char* c_str();
    void clear();
    void getline();
    std::string getBuffer();
    void SetBuf(std::string buffer);
    const char *ToStr();
private:
    std::string buf;
};