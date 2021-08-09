#include "incl.hpp"

Webserver::Webserver() 
	: servers()
	, pfds(nullptr)
	, count(0)
	, OK_repl(new std::string("HTTP/1.1 200 OK\nContent-type: text/html\r\n\r\n"))
	, NotFound_repl(new std::string("HTTP/1.1 404 Not Found\nContent-type: text/html\r\n\r\n"))
	{ std::cout << "Webserver started..." << std::endl; }

Webserver::~Webserver() {
	std::cout << "Webserver stopped..." << std::endl;
	for (auto x : this->servers) {
		delete x;
	}
	if (pfds)
		delete this->pfds;
	delete OK_repl;
	delete NotFound_repl;
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

void    Webserver::sendFile(int to, const char* header, std::size_t hlen, const char* file) {
	char buf[1024] = {0};
	int bytes = 0;
	FILE* fd = fopen(file, "rb");

	if (!fd)
		throw GETException();
	send(to, header, hlen, 0);
	while(!feof(fd)) {
		if ((bytes = fread(buf, 1, 1023, fd)) != 0) {
			buf[bytes] = '\0';
			send(to, buf, bytes, 0);
		}
		else
			break;
	}
	/* rem */std::cout << "<transfer completed>" << std::endl;
	fclose(fd);
}

void    Webserver::doGET_index(int sender) {
	this->sendFile(sender, this->OK_repl->c_str(), this->OK_repl->size(), "index.html");
	close(sender);
}

void    Webserver::doGET(int sender, const char* req, Server* server) {
	if (*req == ' ')
		doGET_index(sender);
	else {
		int i = 0;
		for (; req[i] != ' '; i++) {}
		// i - end of the GET request
		std::string file(req, 0, i);
		file.insert(0, server->getRoot());
		/* rem */std::cout << "GET from FD " << sender << ", from server " << server->getName() << " with path: " << file << std::endl;
		
		this->sendFile(sender, this->OK_repl->c_str(), this->OK_repl->size(), file.c_str());
		close(sender);
	}
}

void    Webserver::doPOST(int sender, const char* req, Server* server) {
	return ;
}

void    Webserver::doPUT(int sender, const char* req, Server* server) {
	return ;
}

void    Webserver::doDELETE(int sender, const char* req, Server* server) {
	return ;
}

void    Webserver::noneMethod(int sender, const char* req, Server* server) {
	return ;
}

void    Webserver::serveReq(const char* buf, int sender, Server* server) {
    auto req = new HTTPRequest(buf, sender, server);
    //auto res = new Response();
    //res->respond(*req);
    delete req;
    //delete res;
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
					/* rem */std::cout << "We got a message from socket " << sender << " with contents:\n" << buf << std::endl;
                    this->serveReq(buf, sender, this->servers[i]);
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

const char* Webserver::GETException::what() const throw() {
	return "Wrong file name, or file is not accessible";
}