#include "incl.hpp"


Response::Response() : header(""), messageBody("") {
	contentTypes[HTML_] = "text/html";
	contentTypes[JPEG_] = "image/jpeg";
	contentTypes[PNG_] = "image/png";
	contentTypes[JSON_] = "application/json";
	contentTypes[XML_] = "application/xml";
	contentTypes[PLAIN_] = "text/plain";
	statuses[0] = "200 OK";
	statuses[1] = "404 Not Found";
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

bool Response::ends_with(const std::string& value, const std::string& ending)
{
    if (ending.size() > value.size())
		return false;
    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::string& Response::getFileType(const std::string& path) {
	std::size_t dot = path.rfind('.');
	if (dot == std::string::npos)
		return this->ctype(PLAIN_);
	std::string ext = path.substr(dot + 1);
	if (ext == "html")
		return this->ctype(HTML_);
	else if (ext == "jpg" || ext == "jpeg")
		return this->ctype(JPEG_);
	else if (ext == "json")
		return this->ctype(JSON_);
	else if (ext == "xml")
		return this->ctype(XML_);
	else
		return this->ctype(PLAIN_);
}

void    Response::sendFile(HTTPRequest_t& req) {
	char buf[1024] = {0};
	int bytes = 0;
	FILE* fd = fopen(req.path.c_str(), "rb");
	/* DEBUG*/
	std::cout << "SENDING FILE: |" << req.path << "|" << std::endl;
	if (!fd) {
		perror("Could not open the file");
		this->header += req.version + " " + this->statuses[1] + "\r\nContent-type: " + this->ctype(HTML_) + "\r\n\r\n";
		/* path to get from config file or req structure */
		fd = fopen("/Users/hrobbin/test/Webserver/www/404_not_found.html", "rb");
		if (!fd)
			throw std::exception();
	}
	else
		this->header += req.version + " " + this->statuses[0] + "\r\nContent-type: " + this->getFileType(req.path) + "\r\n\r\n";
	std::cout << "HEADER:\n" << this->header << "\n"; 
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


void    Response::doGET(HTTPRequest_t& req) {
	std::cout << "request path was: |" << req.path << "|\n";
	if (req.path == "./www/")
		req.path += "index.html";
	this->sendFile(req);
	close(req.sender);
}

void    Response::doPOST(HTTPRequest_t& req) {
	if (ends_with(req.path, "forms.html")) {
		
	}
}

void    Response::doPUT(HTTPRequest_t& req) {}
void    Response::doDELETE(HTTPRequest_t& req) {}
void    Response::noneMethod(HTTPRequest_t& req) {}


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
            this->noneMethod(req);
	}
	catch (std::exception& e) {
		std::cerr << "Undefined error: " << e.what() << std::endl;
		close(req.sender);
	}
}

const char* Response::Exception404::what() const throw() {
	return "Wrong file name, or file is not accessible";
}