# Compiler and flags
CPP := c++
CPP_FLAGS := -Wall -Wextra -Werror -g -std=c++98

# Source and Object files
SRC := main.cpp \
	  channel/Channel.cpp \
	  client/Client.cpp \
	  client/Galois.cpp \
	  client/HTTPClient.cpp\
	  file/File.cpp \
	  server/Server.cpp \
	  server/messageUtils.cpp\
	  server/handleMessage.cpp\
	  server/registerClient.cpp \
	  server/ChannelManager.cpp \
	  server/commands/invite.cpp \
	  server/commands/join.cpp \
	  server/commands/kick.cpp \
	  server/commands/list.cpp \
	  server/commands/mode.cpp \
	  server/commands/part.cpp \
	  server/commands/commands.cpp \
	  server/commands/topic.cpp \
	  server/commands/joke.cpp \
	  server/commands/math.cpp \
	  replies/Replies.cpp \

OBJ := $(SRC:.cpp=.o)

# Executable name
NAME := ircserv

# Default rule
all: $(NAME)

# Linking rule: creating the executable from object files
$(NAME): $(OBJ)
	@echo "$(PURPLE)====================================$(CLR_RESET)"
	@echo "$(GREEN)Compiling of $(BLUE)$(NAME)...$(CLR_RESET)"
	
	$(CPP) $(CPP_FLAGS) $(OBJ) -o $(NAME)

	
	@echo "$(BLUE)$(NAME) $(GREEN)is ready!$(CLR_RESET)"
	@echo "$(PURPLE)====================================$(CLR_RESET)"

# Compiling .cpp files into .o files
%.o: %.cpp
	$(CPP) $(CPP_FLAGS) -c $< -o $@

# Clean up object files
clean:
	rm -f $(OBJ)
	
	clear
	@echo "$(BLUE)$(NAME) object $(GREEN)files have been deleted.$(CLR_RESET)"

# Clean up object files and the executable
fclean: clean
	rm -f $(NAME)
	
	clear
	@echo "$(BLUE)$(NAME) $(GREEN)has been deleted.$(CLR_RESET)"

# Rebuild the project from scratch
re: fclean all
	@echo "$(BLUE)$(NAME) $(GREEN)has been recompiled.$(CLR_RESET)"
	@echo "$(PURPLE)====================================$(CLR_RESET)"

val: re
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./$(NAME) 6667 1234

# .PHONY rule: make targets that aren't filenames
.PHONY: all clean fclean re

# COLORS
CLR_RESET	= \033[0m
RED		    = \033[1;31m
GREEN		= \033[1;32m
YELLOW		= \033[1;33m
BLUE		= \033[1;34m
CYAN 		= \033[1;36m
PURPLE 		= \033[0;35m