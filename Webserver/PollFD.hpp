#ifndef POLLFD_HPP
# define POLLFD_HPP

class PollFD {
	private:
		int     count;
		int     size;
        pollfd* pfds;
		PollFD();
	public:
		PollFD(int count, int size);
		~PollFD();
		PollFD& operator=(const PollFD&);
		PollFD(const PollFD&);
		const pollfd& index(int i) const;
		void    fillServersFDs(std::vector<Server*> servers);
		void    addPollFD(int newFD);
		void    delPollFD(int i);
        pollfd* getInnerPfds();
        int     getCount();
        int     getSize();
};

#endif