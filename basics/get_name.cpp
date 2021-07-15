#include "header.hpp"

int main() {
    struct sockaddr addr;
    socklen_t len = sizeof addr;
    int sockfd = socket(1, 1, 1);
    // The function getpeername() will tell you who is at the other end of a connected stream socket
    int result = getpeername(sockfd, &addr, &len);
    // It returns the name of the computer that your program is running on. The name can then be used by gethostbyname()
    result = gethostname("hostname", 9);
    return 0;
} 