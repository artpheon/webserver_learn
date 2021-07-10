#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;

int main(int ac, char *av[]) {
	int status;
	struct addrinfo hints, *res, *p;
	char ipstr[INET6_ADDRSTRLEN];
	
	if (ac != 2) {
		cerr << "Usage: showip hostname" << endl;
		return 1;
	}
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	if ((status = getaddrinfo(av[1], NULL, &hints, &res)) != 0) {
		cerr << "getaddrinfo: " << gai_strerror(status) << endl;
		return 2;
	}
	cout << "IP addresses for " << av[1] << ":" << endl;
	for (p = res; p != NULL; p = p->ai_next) {
		void* addr;
		char *ipver;
		if (p->ai_family == AF_INET) {
			struct sockaddr_in	*ipv4 = reinterpret_cast<struct sockaddr_in *>(p->ai_addr);
			addr = &(ipv4->sin_addr);
			ipver = "IPv4";
		} else {
			struct sockaddr_in6	*ipv6 = reinterpret_cast<struct sockaddr_in6 *>(p->ai_addr);
			addr = &(ipv6->sin6_addr);
			ipver = "IPv6";
		}
	inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
	cout << ipver << ": " << ipstr << endl;
	}
	freeaddrinfo(res);

	return 0;
}