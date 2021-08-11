#ifndef RESPONSE_HPP
# define RESPONSE_HPP

class HTTPRequest;

class Response {
    private:
        std::string header;
        std::string messageBody;
        enum types_ {
            HTML_ = 0,
            JPEG_,
            PNG_,
            JSON_,
            XML_,
            PLAIN_
        };
        std::string contentTypes[6];
        std::string& ctype(int type);
        void    sendFile(HTTPRequest_t& req);
        void    doGET_index(HTTPRequest_t& req);
        void doGET(HTTPRequest_t& req);
        void doPOST(HTTPRequest_t& req);
        void doPUT(HTTPRequest_t& req);
        void doDELETE(HTTPRequest_t& req);
        void noneMethod(HTTPRequest_t& req);
    public:
        Response();
        ~Response();
        Response(const Response&);
        Response& operator=(const Response&);
        

        void respond(HTTPRequest_t& req);
		class Exception404: public std::exception {
			public:
				virtual const char* what() const throw();
		};
};

#endif