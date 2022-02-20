NAME	= ircserv

SRC		=	srcs/main.cpp srcs/Server.cpp srcs/Client.cpp srcs/Channel.cpp	\
			srcs/Command.cpp

CXX		= c++

CPPFLAGS	= -std=c++98 -Wall -Wextra -Werror

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
