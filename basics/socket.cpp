#include "header.hpp"
/*
int bind(int sockfd, struct sockaddr *my_addr, int addrlen);
sockfd это как раз дескриптор сокета, полученного от socket()
my_addr это инфа об адресе
addrlen длина этого адреса в байтах
*/
void    makeSocket() {
    int sock, stat;
    struct addrinfo hints, *result;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    stat = getaddrinfo("www.google.com", "http", &hints, &result);
    if (stat != 0 || !result) {
        std::cerr << "could not get data: " << gai_strerror(stat) << std::endl;
        exit(1);
    }
    sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol); // returns socket descriptor
    if (sock == -1) {
        perror("Could not create socket");
        exit(1); 
    }
    freeaddrinfo(result);
    close(sock);
}

void    makeSocketAndBind() {
    int sock, stat;
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    // set IP of the host where executed
    hints.ai_flags = AI_PASSIVE;

    stat = getaddrinfo(NULL, "3490", &hints, &res);
    if (stat != 0) {
        std::cerr << "could not get data: " << gai_strerror(stat) << std::endl;
        exit(1);
    }
    // created socket
    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock < 0) {
        perror("Could not create socket");
        exit(1); 
    }
    // binding socket with port got from getaddrinfo
    stat = bind(sock, res->ai_addr, res->ai_addrlen);
    if (stat) {
        perror("Could not bind a socket");
        exit(1);   
    }
    close(sock);
    freeaddrinfo(res);
}

int main() {
    makeSocket();
    makeSocketAndBind();
    return 0;
}