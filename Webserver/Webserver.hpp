#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP
# include "incl.hpp"
# include "Server.hpp"

class Webserver {
    private:
        std::vector<Server*> servers;
        int servNums;
        struct pollfd*      pfds;
        void*   getInetAddress(struct sockaddr* sa);
    public:
        Webserver();
        ~Webserver();
        Webserver(const Webserver&);
        Webserver& operator=(const Webserver&);
        void    describe() const;
        void    addServer(const std::string& port);
        int     run();
        int     invokeServer(int index);

};

#endif