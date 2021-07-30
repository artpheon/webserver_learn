#include "PollFD.hpp"

PollFD::PollFD(int size)
    : count(0)
    , size(size)
    , pfds(size) {}

PollFD::~PollFD() {}

PollFD& PollFD::operator=(const PollFD& rhs) {
    if (this != &rhs) {
        this->count = rhs.count;
        this->size = rhs.size;
        this->pfds = rhs.pfds;
    }
    return *this;
}

PollFD::PollFD(const PollFD& rhs) {
    *this = rhs;
}

const pollfd_t& PollFD::index(int i) const { return this->pfds[i]; }

void    PollFD::fillServersFDs(std::vector<Server*> servers) {
    for (int i = 0; i < this->size; i++) {
        this->pfds[i].fd = servers[i]->getListener();
        this->pfds[i].events = POLLIN;
    }
}

void    PollFD::addPollFD(int newFD) {
    if (this->count == this->size) {
        this->size *= 2;
        this->pfds.resize(this->size);
    }
    this->pfds[this->count].fd = newFD;
    this->pfds[this->count].events = POLLIN;
    this->count += 1;
}

void    PollFD::delPollFD(int i) {
    this->pfds[i] = this->pfds[this->count - 1];
    this->count -= 1;
}
