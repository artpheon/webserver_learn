#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP
# include "incl.hpp"
# include "Server.hpp"
# include "PollFD.hpp"

class Webserver {
    private:
        std::vector<Server*> servers;
        PollFD*     pfds;
        int         count;
        void*       getInetAddress(struct sockaddr* sa);
    public:
        Webserver();
        ~Webserver();
        Webserver(const Webserver&);
        Webserver& operator=(const Webserver&);
        void    describe() const;
        int     invokeServer(int index);
        
        void    addServer(const std::string& port);
        void    setPFD();
        int     run();

};

#endif