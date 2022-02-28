NAME	= ircserv

SRC		=	srcs/main.cpp srcs/Server.cpp srcs/User.cpp srcs/Channel.cpp	\
			srcs/Request.cpp

CXX		= c++

CPPFLAGS	= -g -std=c++98 #-Wall -Wextra -Werror

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
