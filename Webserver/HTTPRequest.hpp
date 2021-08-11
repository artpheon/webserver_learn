#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP
# include "incl.hpp"

class Server;

typedef struct s_HTTPRequest {
    int sender;
    std::string method;
    std::string path;
    std::string version;
    std::string type;
    std::string length;
    std::string connection;
    std::string content;
} HTTPRequest_t;

class HTTPRequest {
    private:
        HTTPRequest_t r;

        std::vector<std::string> split(std::string& s, std::string& delimiter);
        void parseRequest(std::string& s);
        std::string trimLastParam(std::vector<std::string> vec, std::string toFind);
        HTTPRequest();
    public:
        HTTPRequest(const char* buf, int sender, Server* server);
        ~HTTPRequest();
        HTTPRequest(const HTTPRequest&);
        HTTPRequest& operator=(const HTTPRequest&);
        HTTPRequest_t& getRequest();
        class ExceptionInvalidRequest : public std::exception {
            public:
                virtual const char* what() const throw(); 
        };
};

// https://datatracker.ietf.org/doc/html/rfc2616#section-6.1

#endif