#include <poll.h>
#include <iostream>
#include <fstream>
#include <sstream>

int main() {
    struct pollfd pfds[1];

    pfds[0].fd = 0;
    pfds[0].events = POLLIN; // tell me when ready to read

    std::cout << "Hit return or wait 2.5 seconds for timeout" << std::endl;

    int num_event = poll(pfds, 1, -1);
    if (num_event == -1) {
        std::cerr << "poll: error occurred\n";
    }
    else if (num_event == 0) {
        std::cerr << "poll: timeout\n";
    }
    else {
        int polled_in = pfds[0].revents & POLLIN;

        if (polled_in) {
            std::cout << "File descriptor " << pfds[0].fd << " is ready to read" << std::endl;
            std::string tmp;
            std::getline(std::cin, tmp);
            std::cout << tmp;
        }
        else
            std::cout << "Eror occurred: " << pfds[0].revents << std::endl;
    }
    return 0;
}