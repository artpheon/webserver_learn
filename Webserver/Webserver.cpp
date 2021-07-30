#include "Webserver.hpp"

Webserver::Webserver() 
    : servers()
    , pfds(nullptr)
    , count(0)
    { std::cout << "Webserver started..." << std::endl; }

Webserver::~Webserver() {
    std::cout << "Webserver stopped..." << std::endl;
    for (auto x : this->servers) {
        delete x;
    }
    if (pfds)
        delete this->pfds;
}

Webserver::Webserver(const Webserver& ws) {
    *this = ws;
}

Webserver& Webserver::operator=(const Webserver& ws) {
    if (this != &ws) {
        this->servers = ws.servers;
        this->pfds = ws.pfds;
    }
    return *this;
}

void    Webserver::describe() const {
    int i = 1;
    std::cout << "WS contains:\n";
    if (servers.empty())
        std::cout << "(none)" << std::endl;
    else {
        for (auto it = servers.begin(); it != servers.end(); it++) {
            std::cout << i++ << ": " << *it << "\n";
        }
        std::cout << std::endl;
    }   
}

void*	Webserver::getInetAddress(struct sockaddr* sa) {
	if (sa->sa_family == AF_INET)
		return &(reinterpret_cast<struct sockaddr_in*>(sa)->sin_addr);
	else
		return &(reinterpret_cast<struct sockaddr_in6*>(sa)->sin6_addr);
}

void Webserver::addServer(const std::string& port) {
    this->servers.push_back(new Server(port));
    this->count += 1;
}

int Webserver::invokeServer(int index) {
    if (index < 0 || index > this->count)
        return -1;
    if (this->servers[index]->acceptConnection())
        return -1;
    return 0;
}

void    Webserver::setPFD() {
    this->pfds = new PollFD(this->count, this->count);
    this->pfds->fillServersFDs(this->servers);
}

void    Webserver::serverForever() {
    return ;
    // for(;;) {
    //     int pollCount = poll(this->pfds, fdCount, -1);
    //     if (pollCount == -1) {
    //         perror("poll");
    //         exit(1);
    //     }
    //     for (int i = 0; i < fdCount; i++) {
    //         if (pfds[i].revents & POLLIN) { //if its ready to read
    //             if (pfds[i].fd == listener) { // if it was listener
    //                 addrLen = sizeof(remoteAddr);
    //                 newfd = accept(listener, reinterpret_cast<struct sockaddr*>(&remoteAddr), &addrLen);
    //                 if (newfd == -1)
    //                     perror("accept");
    //                 else {
    //                     addToPFDS(&pfds, newfd, &fdCount, &fdSize);
    //                     std::cout << "New connection from "
    //                     << inet_ntop(remoteAddr.ss_family,
    //                                 getInAddr(reinterpret_cast<struct sockaddr *>(&remoteAddr)),
    //                                 remoteIP,
    //                                 INET6_ADDRSTRLEN)
    //                     << " on socket " << newfd << std::endl;
    //                 }
    //             }
    //             else {
    //                 //a regular client
    //                 int senderFD = pfds[i].fd;
    //                 int nbytes = recv(senderFD, buf, sizeof buf, 0);

    //                 if (nbytes <= 0) {
    //                     //error or closed connection
    //                     if (nbytes == 0)
    //                         std::cerr << "socket " << senderFD << " hung up\n";
    //                     if (nbytes < 0)
    //                         perror("recv");
    //                     close(senderFD);
    //                     delFromPFDS(pfds, i, &fdCount);
    //                 }
    //                 else {
    //                     // good data

    //                     for (int j = 0; j < fdCount; j++) {
    //                         int destFD = pfds[j].fd;
    //                         if (destFD != listener && destFD != senderFD) {
    //                             if (send(destFD, buf, nbytes, 0) != 0)
    //                                 perror("send");
    //                         }
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // }
}


int     Webserver::run() {
    try {
        addServer("8080");
        addServer("3490");
        setPFD();
        serverForever();
    }
    catch(std::exception& e) {
        (void)e;
        std::cerr << "Error occurred while running the server\n";
        return 1;
    }
    return 0;
}