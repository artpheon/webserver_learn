#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT "3490"
#define MAX_DATA_SIZE 100


void* getInAddr(struct sockaddr* sa) {
    if (sa->sa_family == AF_INET)
        return &(reinterpret_cast<struct sockaddr_in*>(sa)->sin_addr);
    else
        return &(reinterpret_cast<struct sockaddr_in6*>(sa)->sin6_addr);
}

int main(int argc, char *argv[]) {
    int sockfd, numbytes;
    char buf[MAX_DATA_SIZE];
    struct addrinfo hints, *servinfo, *p;
    int rval;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2) {
        std::cerr << "usage: client hostname\n";
        return 1;
    }
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    rval = getaddrinfo(argv[1], PORT, &hints, &servinfo);
    if (rval) {
        std::cerr << "getaddrinfo failed:" << gai_strerror(rval);
        return 1;
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("socket");
            continue;
        }
        if ((connect(sockfd, p->ai_addr, p->ai_addrlen)) == -1) {
            perror("client: connect");
            close(sockfd);
            continue;
        }
        break;
    }

    if (p == NULL) {
        std::cerr << "client: failed to connect\n";
        return 2;
    }

    inet_ntop(p->ai_family, getInAddr(p->ai_addr), s, sizeof(s)); // заполнит буфер адресом в виде строки
    std::cout << "client: connecting to " << s << std::endl;
    
    freeaddrinfo(servinfo);

    if ((numbytes = recv(sockfd, buf, MAX_DATA_SIZE - 1, 0)) == -1) {
        perror("recv");
        return 1;
    }
    buf[numbytes] = '\0';
    std::cout << "client received: " << buf << std::endl;
    close(sockfd);
    return 0;

}