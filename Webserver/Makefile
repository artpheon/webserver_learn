NAME	= webserv
SRC		= main.cpp Server.cpp Webserver.cpp helpers.cpp PollFD.cpp
INC		= Server.hpp Webserver.hpp incl.hpp helpers.hpp PollFD.hpp
OBS		= $(SRC:.cpp=.o)
CC		= clang++
FL		= --std=c++11

%.o:	%.cpp $(INC)
	$(CC) $(FL) -c $< -o $(<:.cpp=.o)

all:	$(NAME)

$(NAME):	$(OBS) 
	$(CC) $(FL) $(OBS) -o $(NAME)

clean:	
	rm -f $(OBS)
fclean: clean
	rm -f $(NAME)
re:	fclean all

.PHONY: all clean fclean re