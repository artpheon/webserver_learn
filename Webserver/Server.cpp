#include "Server.hpp"

Server::Server() {}

int	Server::getListener(const std::string& port) {
	struct addrinfo		hints, *ai;
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
	for (struct addrinfo* p = ai; p != NULL; p = p->ai_next) {
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
		freeaddrinfo(ai);
		if (p == NULL) {
			std::cerr << "server failed to bind" << std::endl;
			return -1;
		}
		if (listen(sockfd, BACKLOG) == -1) {
			perror("server: listen");
			return -1;
		}
	}
	return sockfd;
}

Server::Server(const std::string& port) {
	this->listener = this->getListener(port);
	this->pfds = nullptr;
	std::cout << "Server N/A started on port " << port << " with socketFD: " << this->listener << std::endl;
}

Server::~Server() {}

Server::Server(const Server& rhs) {
	*this = rhs;
}

Server& Server::operator=(const Server& rhs) {
	if (this != &rhs) {
		this->listener = rhs.listener;
		this->pfds = rhs.pfds;
	}
	return *this;
}