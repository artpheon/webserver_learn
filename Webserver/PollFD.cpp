#include "PollFD.hpp"

PollFD::PollFD(int count, int size) {
    this->count = count;
    this->size = size;
    this->pfds = new pollfd_t[size];
    //заполнить фд-шниками из серверов
}

PollFD::~PollFD() {

}

PollFD& PollFD::operator=(const PollFD&) {

}

PollFD::PollFD(const PollFD&) {

}

struct pollfd& PollFD::index(int i) {

}

void    PollFD::addPollFD(int newFD) {

}

void    PollFD::delPollFD(int i) {

}
