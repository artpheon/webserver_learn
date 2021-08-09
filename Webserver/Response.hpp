#ifndef RESPONSE_HPP
# define RESPONSE_HPP
# include "incl.hpp"

class HTTPRequest;

class Response {
    private:
        std::string header;
        std::string messageBody;
        enum contTypes {
            HTML_ = 0,
            JPEG_,
            PNG,
            JSON_,
            XML_
        };
    public:
        void respond(HTTPRequest& req);

};

#endif