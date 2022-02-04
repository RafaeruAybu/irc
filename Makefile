NAME	= ircserv
SRC		= srcs/main.cpp srcs/Server.cpp srcs/Client.cpp srcs/Channel.cpp srcs/ParseConfig.cpp
CXX		= c++
CPPFLAGS	= -Wall -Wextra -Werror -std=c++98
OBJ		= $(SRC:.cpp=.o)

$(NAME): $(OBJ)
	$(CXX) $(CPPFLAGS) $(OBJ) -o $(NAME) 

all: $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean
