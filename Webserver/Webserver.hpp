#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

class Server;
class PollFD;

class Webserver {
	private:
		std::vector<Server*> servers;
		PollFD*     pfds;
		int         count;
		const std::string* OK_repl;
		const std::string* NotFound_repl;
		void*       getInetAddress(struct sockaddr* sa);
		void    serveReq(const char* buf, int to, Server* server);
        //
		void    sendFile(int to, const char* header, std::size_t hlen, const char* file);
		void    doGET_index(int sender);
        //
	public:
		Webserver();
		~Webserver();
		Webserver(const Webserver&);
		Webserver& operator=(const Webserver&);
		void    describe() const;
		int     invokeServer(int index);
		void    addServer(const std::string& port, const std::string& root = "./", const std::string& name = "N/A");
		void    setPFD();
		void    serverForever();

		class WebservExceptServFailed: public std::exception {
			private:
				std::string& port;
				std::string& name;
			public:
				WebservExceptServFailed(std::string& port, std::string& name);
				virtual const char* what() const throw();
		};
};

#endif