#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP
# include "incl.hpp"
# include "Server.hpp"

class Webserver {
    private:
        std::vector<Server*> servers;
        int servNums;
    public:
    Webserver();
    Webserver(int num);
    ~Webserver();
    Webserver(const Webserver&);
    Webserver& operator=(const Webserver&);
    void    describe() const;
    void*   getInetAddress(struct sockaddr* sa);
    void    addServer(const std::string& port);
};

#endif