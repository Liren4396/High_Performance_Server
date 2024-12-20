#include "include/Manager.h"

Manager::Manager() = default;

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

std::vector<int> Manager::getFds() {
    return fds;
}

void Manager::append(int fd) {
    Manager::getInstance().fds.push_back(fd);
}