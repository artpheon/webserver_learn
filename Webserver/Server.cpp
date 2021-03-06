#include "incl.hpp"


int	Server::getListenerSocket(const std::string& port) {
	struct addrinfo		hints, *ai, *p;
	int		sockfd = 0;

	std::memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	int retval = getaddrinfo(NULL, port.c_str(), &hints, &ai);
	if (retval != 0) {
		std::cerr << "getaddrinfo: " << gai_strerror(retval) << std::endl;
		return -1;
	}
	int yes = 1;
	for (p = ai; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			return -1;
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}
	freeaddrinfo(ai);
	if (p == NULL) {
		std::cerr << "server failed to bind" << std::endl;
		return -1;
	}
	if (listen(sockfd, BACKLOG) == -1) {
		perror("server: listen");
		return -1;
	}
	return sockfd;
}

Server::Server(const std::string& port, const std::string& root, const std::string& name)
    : listener(0)
    , port(port)
    , name(name)
    , root(root) {
	this->listener = this->getListenerSocket(port);
	if (this->listener < 0)
		std::cerr << "server " << this->name << " failed to build" << std::endl;
	else
		std::cout << "server <" << this->name << "> started on port " << this->port << " with socketFD: " << this->listener << std::endl;
}

Server::~Server() {
	close(this->listener);
}

Server::Server(const Server& rhs): Server(rhs.port, rhs.root, rhs.name) {}

Server& Server::operator=(const Server& rhs) {
	*this = Server(rhs.port, rhs.root, rhs.name);
	return *this;
}

int    Server::acceptConnection() {
	struct sockaddr_storage addr;
	socklen_t slen = sizeof(addr);

	int newFD = accept(this->listener, reinterpret_cast<struct sockaddr*>(&addr), &slen);
	if (newFD == -1) {
		perror("accept");
		return -1;
	}

	char buf[1024];
	int nbytes = recv(newFD, buf, sizeof(buf), 0);
	if (nbytes == -1) {
		perror("recv");
		return -1;
	}
	buf[nbytes] = '\0';
	std::cout << buf << std::endl;
	close(newFD);
	return 0;
}

int		Server::getListener() const { return this->listener; }
const   std::string& Server::getName() const { return this->name;}

const   std::string& Server::getPort() const { return this->port;}

const   std::string& Server::getRoot() const { return this->root;}
