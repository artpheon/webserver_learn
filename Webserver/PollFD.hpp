#ifndef POLLFD_HPP
# define POLLFD_HPP
# include "incl.hpp"
# include "Server.hpp"

struct pollfd_t {
	int     fd;
	short   events;
	short   revents;
};

class PollFD {
	private:
		int count;
		int size;
		std::vector<pollfd_t> pfds;
		PollFD();
	public:
		PollFD(int size);
		~PollFD();
		PollFD& operator=(const PollFD&);
		PollFD(const PollFD&);
		const pollfd_t& index(int i) const;
		void    fillServersFDs(std::vector<Server*> servers);
		void    addPollFD(int newFD);
		void    delPollFD(int i);
};

#endif