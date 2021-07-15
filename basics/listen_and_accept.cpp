#include "header.hpp"

/*
I don’t want to connect to a remote host, i only LISTEN ports for incoming connections.
1. Listen
2. Accept
int listen(int sockfd, int backlog);
backlog - how many connections are allowed to wait in line. Connections will wait until accept()
ususally 20, but can be of 5-10

To listen for incoming connections:
1 getaddrinfo();
2 socket();
3 bind();
4 listen();
5 accept() goes here
*/

/*
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
accept() accepts a connection and returns a new filedes. The old filedes still listening the port
the new filedes is ready to send() and recv()
*/
#define MYPORT "3490"
#define BACKLOG 10

void    unbind(int listener) {
    int yes = 1;
    if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
        perror("setsockopt");
        exit(1);
    }
}

int main() {
    struct sockaddr_storage their_addr;
    socklen_t addr_size;
    struct addrinfo hints, *res;
    int sockfd, new_fd, stat;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    stat = getaddrinfo(NULL, MYPORT, &hints, &res);
    if (stat != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(stat);
        exit(1);
    }
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket() failed");
        exit(1);
    }
    stat = bind(sockfd, res->ai_addr, res->ai_addrlen);
    if (stat != 0) {
        perror("bind() failed");
        exit(1);
    }
    stat = listen(sockfd, BACKLOG);
    if (stat != 0) {
        perror("listen() failed");
        exit(1);
    }
    addr_size = sizeof(their_addr);
    new_fd = accept(sockfd, reinterpret_cast<struct sockaddr*>(&their_addr), &addr_size);

    close(new_fd);
    //unbind(sockfd);
    close(sockfd);
    freeaddrinfo(res);
    return 0;
}