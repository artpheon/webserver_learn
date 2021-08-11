#include "incl.hpp"


Response::Response() : header(""), messageBody("") {
	contentTypes[HTML_] = "text/html";
	contentTypes[JPEG_] = "image/jpeg";
	contentTypes[PNG_] = "image/png";
	contentTypes[JSON_] = "application/json";
	contentTypes[XML_] = "application/xml";
	contentTypes[PLAIN_] = "text/plain";
}

Response::~Response() {}

Response::Response(const Response& rhs) {
	*this = rhs;
}

Response& Response::operator=(const Response& rhs) {
	if (this != &rhs) {
		this->header = rhs.header;
		this->messageBody = rhs.messageBody;
	}
	return *this;
}

std::string& Response::ctype(int type) {
	return this->contentTypes[type];
}

void    Response::sendFile(HTTPRequest_t& req) {
	char buf[1024] = {0};
	int bytes = 0;
	FILE* fd = fopen(req.path.c_str(), "rb");

	if (!fd)
		throw std::exception();
	this->header += req.version + " 200 OK\r\nContent-type: " + this->ctype(HTML_) + "\r\n\r\n";
	send(req.sender, this->header.c_str(), this->header.size(), 0);
	while(!feof(fd)) {
		if ((bytes = fread(buf, 1, 1023, fd)) != 0) {
			buf[bytes] = '\0';
			send(req.sender, buf, bytes, 0);
		}
		else
			break;
	}
	/* rem */std::cout << "<transfer completed>" << std::endl;
	fclose(fd);
}

void    Response::doGET_index(HTTPRequest_t& req) {
	req.path += "index.html";
	this->sendFile(req);
	close(req.sender);
}


void    Response::doGET(HTTPRequest_t& req) {
	if (req.path == "/")
		doGET_index(req);
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

void Response::respond(HTTPRequest_t& req) {
    try {
		if (req.method == "GET") {
			this->doGET(req);
		}
		else if (req.method == "POST") {
			this->doPOST(req);
		}
        else if (req.method == "PUT") {
			this->doPUT(req);
		}
        else if (req.method == "DELETE") {
			this->doDELETE(req);
		}
        else
            this->noneMethod(req):
	}
	catch (Response::Exception404& e) {
		std::cerr << e.what() << ": " << strerror(errno) << std::endl;
		this->sendFile(req.sender, this->NotFound_repl->c_str(), this->NotFound_repl->size(), "404_not_found.html");
		close(req.sender);
	}
	catch (std::exception& e) {
		std::cerr << "Undefined error: " << e.what() << std::endl;
	}
}

const char* Response::Exception404::what() const throw() {
	return "Wrong file name, or file is not accessible";
}