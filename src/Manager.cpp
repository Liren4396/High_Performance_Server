// Manager.cpp
#include "include/Manager.h"

Manager::Manager() = default;

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

std::vector<int> Manager::getFds() {
    return fds;
}

void Manager::remove(int fd) {
    auto it = std::remove(fds.begin(), fds.end(), fd);
    fds.erase(it, fds.end());
}

void Manager::append(int fd) {
    Manager::getInstance().fds.push_back(fd);
}