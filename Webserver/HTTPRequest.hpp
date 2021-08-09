#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP
# include "incl.hpp"

class Server;

class HTTPRequest {
    private:
        int sender;
        std::string method;
        std::string path;
        std::string version;
        std::string type;
        std::string length;
        std::string connection;
        std::string content;

        std::vector<std::string> split(std::string& s, std::string& delimiter);
        void parseRequest(std::string& s);
        std::string trimLastParam(std::vector<std::string> vec, std::string toFind);
        HTTPRequest();
    public:
        HTTPRequest(const char* buf, int sender, Server* server);
        ~HTTPRequest();
        HTTPRequest(const HTTPRequest&);
        HTTPRequest& operator=(const HTTPRequest&);
        class ExceptionInvalidRequest : public std::exception {
            public:
                virtual const char* what() const throw(); 
        };
};

#endif