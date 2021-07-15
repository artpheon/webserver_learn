#include "header.hpp"
/*
int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);
sockfd - socket fd to connect to
serv_addr - address
addr_len - address length
*/

void connectSock() {
    int sockfd, stat;
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);
    //any type
    hints.ai_family = AF_UNSPEC;
    //stream socket
    hints.ai_socktype = SOCK_STREAM;
    //get info on google on port 3490
    stat = getaddrinfo("172.0.0.1", "3490", &hints, &res);
    if (stat != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(stat) << std::endl;
        exit(1);
    }
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket() failed");
        exit(1);
    }
    //connect using filedes with address in RES
    stat = connect(sockfd, res->ai_addr, res->ai_addrlen);
    if (stat != 0) {
        perror("connect() failed");
        exit(1);
    }
    close(sockfd);
    freeaddrinfo(res);
}

int main() {
    connectSock();
    return 0;
}