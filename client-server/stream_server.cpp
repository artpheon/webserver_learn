#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#define PORT "3490"
#define BACKLOG 10

void	childSigHandler(int s) {
	static_cast<void>(s);
	
	int errnotmp = errno;

	while (waitpid(-1, NULL, WNOHANG) > 0);
	
	errno = errnotmp;
}

void*	getInAddr(struct sockaddr* sa) {
	if (sa->sa_family == AF_INET)
		return &(reinterpret_cast<struct sockaddr_in*>(sa)->sin_addr);
	else
		return &(reinterpret_cast<struct sockaddr_in6*>(sa)->sin6_addr);
}

int main() {
	struct addrinfo		hints, *servinfo, *p;
	int sockfd, newfd;
	struct sigaction	sa;
	int	yes = 1, retval;

	std::memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	retval = getaddrinfo(NULL, PORT, &hints, &servinfo);
	if (retval != 0) {
		std::cerr << "getaddrsinfo: " << gai_strerror(retval) << std::endl;
		return 1;
	}
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}
	freeaddrinfo(servinfo);
	if (p == NULL) {
		std::cerr << "server failed to bind" << std::endl;
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("server: listen");
		exit(1);
	}
	sa.sa_handler = childSigHandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
	std::cout << "Waiting for connection..." << std::endl;
	char s[INET6_ADDRSTRLEN];
	struct sockaddr_storage theirAddr;
	socklen_t sinSize = sizeof(theirAddr);

	while(1) {
		newfd = accept(sockfd, reinterpret_cast<struct sockaddr*>(&theirAddr), &sinSize);
		if (newfd == -1) {
			perror("accept");
			continue;
		}
		inet_ntop(theirAddr.ss_family,
		getInAddr(reinterpret_cast<struct sockaddr*>(&theirAddr)),
		s,
		sizeof(s));
		std::cout << "Server got connection from " << s << std::endl;
		if (!fork()) {
			close(sockfd);
			retval = send(newfd, "HTTP/1.1 200 OK\nContent-type: text/html\r\n\r\nHello, world!", 57, 0);
			if (retval == -1)
				perror("send");
			close(newfd);
			exit(0);
		}
		close(newfd);
	}
	return 0;
}