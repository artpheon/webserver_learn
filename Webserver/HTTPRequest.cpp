#include "incl.hpp"


std::vector<std::string> HTTPRequest::split(std::string& s, std::string& delimiter) {
    std::size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    res.push_back(s.substr(pos_start));
    return res;
}


void HTTPRequest::parseRequest(std::string& s) {
    std::string del = "\r\n";
    std::vector<std::string> lines = split(s, del);
    del = " ";
    std::vector<std::string> head = split(lines[0], del);
    this->r.method = head[0];
    this->r.path = head[1].replace(0, 1, "./www/");
    /*   to add PATH from config */
    this->r.version = head[2];
    this->r.content = lines.back();
    this->r.type = trimLastParam(lines, "Content-Type:");
    this->r.length = trimLastParam(lines, "Content-Length:");
    this->r.connection = trimLastParam(lines, "Connection:");
}

std::string HTTPRequest::trimLastParam(std::vector<std::string> vec, std::string toFind) {
    std::vector<std::string>::iterator it;
    
    for (it = vec.begin(); it != vec.end(); it++) {
        if ((*it).find(toFind) == 0)
            break;
    }
    if (it == vec.end())
        return "";
    std::string &s = *it;
    return s.substr(s.rfind(" ") + 1);
}

HTTPRequest::HTTPRequest(const char* buf, int sender, Server* server) {
    this->r.sender = sender;
    std::string s(buf);
    try {
        this->parseRequest(s);
    }
    catch (ExceptionInvalidRequest& e) {
        std::cerr << e.what() << std::endl;
    }
    catch (std::exception& e) {
        std::cerr << "undefined error: " << e.what() << std::endl;
    }
}

HTTPRequest::~HTTPRequest() {}

HTTPRequest::HTTPRequest(const HTTPRequest& rhs) {
    *this = rhs;
}

HTTPRequest& HTTPRequest::operator=(const HTTPRequest& rhs) {
    if (this != &rhs) {
        this->r.sender = rhs.r.sender;
        this->r.method = rhs.r.method;
        this->r.path = rhs.r.path;
        this->r.version = rhs.r.version;
        this->r.type = rhs.r.type;
        this->r.length = rhs.r.length;
        this->r.connection = rhs.r.connection;
        this->r.content = rhs.r.content;
    }
    return *this;
}

HTTPRequest_t& HTTPRequest::getRequest() {
    return this->r;
}


const char* HTTPRequest::ExceptionInvalidRequest::what() const throw() {
    return "Request is invalid";
}