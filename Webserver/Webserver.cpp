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
    this->pfds = new PollFD(this->count);
    this->pfds->fillServersFDs(this->servers);
}


int     Webserver::run() {
    try {
        addServer("8080");
        addServer("3490");
        setPFD();
    }
    catch(std::exception& e) {
        (void)e;
        std::cerr << "Error occurred while running the server\n";
        return 1;
    }
    return 0;
}