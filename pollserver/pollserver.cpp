#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

#define PORT "9034"

int getListenerSocket() {
    int listener, retVal, yes = 1;
    struct addrinfo hints, *ai, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    retVal = getaddrinfo(NULL, PORT, &hints, &ai);
    if (retVal != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(retVal) << std::endl;
        exit(1);
    }

    for (p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0)
            continue;

        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }
    freeaddrinfo(ai);
    
    if (p == NULL)
        return -1;

    if (listen(listener, 10) != 0)
        return -1;
    
    return listener;
}

void *getInAddr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET)
        return &(reinterpret_cast<struct sockaddr_in*>(sa)->sin_addr);
    else
        return &(reinterpret_cast<struct sockaddr_in6*>(sa)->sin6_addr);
}

void addToPFDS(struct pollfd** pfds, int newfd, int *fdCount, int* fdSize) {
    if (*fdCount == *fdSize) {
        *fdSize *= 2;
        *pfds = static_cast<struct pollfd*>(realloc(*pfds, sizeof(**pfds) * *fdSize));
    }

    (*pfds)[*fdCount].fd = newfd;
    (*pfds)[*fdCount].events = POLLIN;
    ++(*fdCount);
}

void    delFromPFDS(struct pollfd pfds[], int i, int* fdCount) {
    pfds[i] = pfds[*fdCount - 1];
    (*fdCount) -= 1;
}

int main() {
    int listener, newfd; // listening socket FD and new (client's) FD
    struct sockaddr_storage remoteAddr; // storing client's addr
    socklen_t addrLen; // address length
    char buf[256]; // buffer for client data
    char remoteIP[INET6_ADDRSTRLEN];

    int fdCount = 0;
    int fdSize = 5;
    struct pollfd* pfds = static_cast<struct pollfd*>(malloc(sizeof(*pfds) * fdSize));

    listener = getListenerSocket();
    if (listener == -1) {
        std::cerr << "Error getting listening socket\n";
        exit(1);
    }
    pfds[0].fd = listener;
    pfds[0].events = POLLIN;
    fdCount = 1;

    for(;;) {
        int pollCount = poll(pfds, fdCount, -1);
        if (pollCount == -1) {
            perror("poll");
            exit(1);
        }
        for (int i = 0; i < fdCount; i++) {
            if (pfds[i].revents & POLLIN) { //if its ready to read
                if (pfds[i].fd == listener) { // if it was listener
                    addrLen = sizeof(remoteAddr);
                    newfd = accept(listener, reinterpret_cast<struct sockaddr*>(&remoteAddr), &addrLen);
                    if (newfd == -1)
                        perror("accept");
                    else {
                        addToPFDS(&pfds, newfd, &fdCount, &fdSize);
                        std::cout << "New connection from "
                        << inet_ntop(remoteAddr.ss_family,
                                    getInAddr(reinterpret_cast<struct sockaddr *>(&remoteAddr)),
                                    remoteIP,
                                    INET6_ADDRSTRLEN)
                        << " on socket " << newfd << std::endl;
                    }
                }
                else {
                    //a regular client
                    int senderFD = pfds[i].fd;
                    int nbytes = recv(senderFD, buf, sizeof buf, 0);

                    if (nbytes <= 0) {
                        //error or closed connection
                        if (nbytes == 0)
                            std::cerr << "socket " << senderFD << " hung up\n";
                        if (nbytes < 0)
                            perror("recv");
                        close(senderFD);
                        delFromPFDS(pfds, i, &fdCount);
                    }
                    else {
                        // good data

                        for (int j = 0; j < fdCount; j++) {
                            int destFD = pfds[i].fd;
                            if (destFD != listener && destFD != senderFD) {
                                if (send(destFD, buf, nbytes, 0) != 0)
                                    perror("send");
                            }
                        }
                    }
                }
            }
        }
    }
}