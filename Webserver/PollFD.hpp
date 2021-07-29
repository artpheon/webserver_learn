#ifndef POLLFD_HPP
# define POLLFD_HPP
# include "incl.hpp"

class PollFD {
    private:
        struct pollfd* pfds;
};

#endif;