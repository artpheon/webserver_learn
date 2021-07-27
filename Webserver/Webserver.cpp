#include "Webserver.hpp"

Webserver::Webserver() 
    : servers()
    , servNums(0)
    { std::cout << "Webserver started..." << std::endl; }

Webserver::Webserver(int num)
    : servers()
    , servNums(num)
    { std::cout << "Webserver started..." << std::endl; }

Webserver::~Webserver()
    { std::cout << "Webserver stopped..." << std::endl; }

Webserver::Webserver(const Webserver& ws) {
    *this = ws;
}

Webserver& Webserver::operator=(const Webserver& ws) {
    if (this != &ws) {
        this->servers = ws.servers;
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
}