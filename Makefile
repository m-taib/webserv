FLAGS= -Wextra -Werror -Wall -std=c++98 
NAME= webserve

REQ_RES_DIR = request_response/
OBJ_DIR = obj/
SERVER_DIR = server/
COOKIES_DIR = cookies/

REQ_RES_SRC = request.cpp request_header.cpp request_line.cpp response.cpp response_header.cpp response_line.cpp  auto_index.cpp
SERVER_SRC = webserve.cpp config_file.cpp CommonDirectives.cpp client.cpp
#COOKIES_DIR = 


OSRC = $(addprefix $(OBJ_DIR), $(REQ_RES_SRC:.cpp=.o))
OSRC += $(addprefix $(OBJ_DIR), $(SERVER_SRC:.cpp=.o))
#OSRC += $(addprefix $(OBJ_DIR), $(COOKIES_DIR:.c=.o))

INCLUDES= -I includes


all : $(NAME)

$(NAME) : $(OSRC) 
	c++  $^ -o $(NAME)

$(OBJ_DIR)%.o: $(REQ_RES_DIR)%.cpp includes/*.hpp
	@mkdir -p $(OBJ_DIR)
	c++  -c $< -o $@

$(OBJ_DIR)%.o: $(SERVER_DIR)%.cpp includes/*.hpp
	@mkdir -p $(OBJ_DIR)
	c++  -c $< -o $@

# $(OBJ_DIR)%.o: $(COOKIES_DIR)%.c includes/*.hpp
# 	@mkdir -p $(OBJ_DIR)
# 	@$(CC) -Wall -Wextra -Werror -c $< -o $@

clean :
	@rm -rf $(OBJ_DIR)
fclean : clean
	@rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re 