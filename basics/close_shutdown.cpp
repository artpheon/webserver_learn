#include "header.hpp"

int main() {
    int sockfd = socket(1, 1, 1);
    int stat = 0;

    stat = shutdown(sockfd, 0); // receives are disallowed
    shutdown(sockfd, 1); // no sends
    shutdown(sockfd, 2); // just like close()
    // shutdown() does not actually close the FD, just changes its usability
    return 0;
}