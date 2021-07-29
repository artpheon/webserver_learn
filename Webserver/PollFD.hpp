#ifndef POLLFD_HPP
# define POLLFD_HPP
# include "incl.hpp"


struct pollfd_t {
	int     fd;
	short   events;
	short   revents;
};

class PollFD {
    private:
        int count;
        int size;
        pollfd_t *pfds;
        PollFD();
    public:
        PollFD(int count, int size);
        ~PollFD();
        PollFD& operator=(const PollFD&);
        PollFD(const PollFD&);
        struct pollfd& index(int i);
        void    addPollFD(int newFD);
        void    delPollFD(int i);
};

#endif;