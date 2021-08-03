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

void Webserver::addServer(const std::string& port, const std::string& root, const std::string& name) {
    this->servers.push_back(new Server(port, root, name));
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

void    Webserver::doGET(int s, const char* req, Server* serv) {
    int i = 0;
    for (; req[i] != ' '; i++) {}
    std::string obj(req, 0, i);
    std::string path = serv->getRoot() + obj;
    std::cout << "GET from FD " << s << ", from server " << serv->getName() << " with path: " << path << std::endl;
    
    char buf[1024] = {0};
    std::size_t rret, wret;
    FILE* fd = fopen(obj.c_str(), "rb");
    if (!fd)
        return ;
    int bytes;
    while(!feof(fd)) {
        if ((bytes = fread(&buf, 1, 1023, fd)) != 0)
            send(s, buf, bytes, 0);
        else
            break;
    }
    fclose(fd);

    // std::fstream fs;
    // fs.open(obj, std::fstream::in);
    // std::string header = "HTTP/1.1 200 OK\nContent-type: text/html\r\n\r\n";
    // while (!fs.eof())
    // if (send(sender, buffer_reply.c_str(), buffer_reply.size(), 0) == -1)
    //     perror("send");
    // close(sender);
}

void    Webserver::serverForever() {
    char buf[1024] = {0};
    int sender = -1, nbytes = -1, pollCount = -1;
    struct sockaddr_storage them;
    socklen_t len = sizeof(them);

    for(;;) {
        pollCount = poll(pfds->getInnerPfds(), pfds->getCount(), -1);
        if (pollCount == -1) {
            perror("poll");
            exit(1);
        }
        for (int i = 0; i < pfds->getCount(); i++) {
            if (pfds->index(i).revents & POLLIN) { //if its ready to read
                sender = accept(pfds->index(i).fd, reinterpret_cast<struct sockaddr*>(&them), &len);
                nbytes = recv(sender, buf, sizeof(buf) - 1, 0);
                if (nbytes <= 0) {
                    //error or closed connection
                    if (nbytes == 0)
                        std::cerr << "socket " << sender << " hung up\n";
                    if (nbytes < 0)
                        perror("recv");
                    close(sender);
                }
                else {
                    buf[nbytes] = '\0';
                    std::cout << "We got a message from socket " << sender
                    << " with contents:\n" << buf << std::endl;
                    // good data
                    // if (send(destFD, buf, nbytes, 0) != 0)
                    //     perror("send");
                    if ((strncmp(buf, "GET", 3)) == 0)
                        this->doGET(sender, &buf[5], this->servers[i]);
                }
            }
        }
    }
} // END serverForever

Webserver::WebservExceptServFailed::WebservExceptServFailed(std::string& port, std::string& name) : port(port), name(name) {}

const char* Webserver::WebservExceptServFailed::what() const throw() {
    std::string msg = "Server " + this->name + " on port " + this->port + " was not created\n";
    return msg.c_str();
}