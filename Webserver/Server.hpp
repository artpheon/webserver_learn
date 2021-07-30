#ifndef SERVER_HPP
# define SERVER_HPP
# include "incl.hpp"
# define BACKLOG 10

class Server {
	private:
		int     getListenerSocket(const std::string& port);
		int     listener;

		Server();
	public:
		Server(const std::string& port);
		~Server();
		Server(const Server&);
		Server& operator=(const Server&);
		int		acceptConnection();
		int		getListener() const;
};

#endif