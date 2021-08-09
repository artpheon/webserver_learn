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
    this->method = head[0];
    this->path = head[1];
    this->version = head[2];
    this->content = lines.back();
    this->type = trimLastParam(lines, "Content-Type:");
    this->length = trimLastParam(lines, "Content-Length:");
    this->connection = trimLastParam(lines, "Connection:");
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
    this->sender = sender;
    std::string s(buf);
    try {
        this->parseRequest(s);
    }
    catch (ExceptionInvalidRequest& e) {
        std::cerr << e.what() << std::endl;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    std::cout << "Result:\nMethod: |" << this->method <<
    "|,\n Type: |" << this->type <<
    "|,\n Path: |" << this->path <<
    "|,\n Version: |" << this->version <<
    "|,\n Connection: |" << this->connection << 
    "|,\n Content: |" << this->content <<
    "|,\n Length: |" << this->length <<
    "|" << std::endl;
}

HTTPRequest::~HTTPRequest() {

}

HTTPRequest::HTTPRequest(const HTTPRequest&) {

}

HTTPRequest& HTTPRequest::operator=(const HTTPRequest&) {
    return *this;
}


const char* HTTPRequest::ExceptionInvalidRequest::what() const throw() {
    return "Request is invalid";
}