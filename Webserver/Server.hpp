#ifndef SERVER_HPP
# define SERVER_HPP
# include "incl.hpp"
# define BACKLOG 10

class Server {
	private:
		int     listener;
        const std::string& port;
        const std::string& name;
        const std::string& root;

		int     getListenerSocket(const std::string& port);
        
		Server();
	public:
		Server(const std::string& port, const std::string& root, const std::string& name);
		~Server();
		Server(const Server&);
		Server& operator=(const Server&);
		int		acceptConnection();
		int		getListener() const;
        const   std::string& getName() const;
        const   std::string& getPort() const;
        const   std::string& getRoot() const;
};

#endif