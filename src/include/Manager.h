#include <vector>
#include <string>
class Manager {
public:
    static Manager& getInstance();
    std::vector<int> getFds();
    void append(int fd);
    void setHash(int fd, std::string name);
private:
    std::vector<int> fds;
    Manager();
    Manager(const Manager&) = delete;
    Manager& operator=(const Manager&) = delete;
};
