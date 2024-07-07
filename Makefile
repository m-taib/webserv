
SRC = webserve.cpp request_line.cpp request_header.cpp  request.cpp 
OSRC = $(SRC:.cpp=.o)

NAME = webserve
CFLAGS = -std=c++98

%.o:%.cpp webserve.hpp request_header.hpp request_line.hpp request.hpp 
	c++ -c $(CFLAGS) $<

all: $(NAME)

$(NAME) :$(OSRC)
		c++ $(CFLAGS) $^ -o $(NAME)

clean :
	@rm -f *.o
fclean :
	@rm -f *.o $(NAME) 
re : fclean all

.PHONY : all clean fclean re
