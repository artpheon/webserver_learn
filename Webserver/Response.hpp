#ifndef RESPONSE_HPP
# define RESPONSE_HPP

typedef struct s_HTTPRequest HTTPRequest_t;

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
        std::string statuses[2];
        std::string& getFileType(const std::string& path);
        std::string& ctype(int type);
        bool ends_with(const std::string& value, const std::string& ending);
        void    sendFile(HTTPRequest_t& req);
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