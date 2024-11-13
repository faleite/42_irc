# Compiler and flags
CPP := c++
CPP_FLAGS := -Wall -Wextra -Werror -g -std=c++98

# Source and Object files
SRC := main.cpp \
	  channel/Channel.cpp \
	  client/Client.cpp \
	  client/HTTPClient.cpp\
	  file/File.cpp \
	  server/Server.cpp \
	  server/messageUtils.cpp\
	  server/handleMessage.cpp\
	  server/connectRegister.cpp \
	  server/ChannelManager.cpp \
	  server/commands/invite.cpp \
	  server/commands/join.cpp \
	  server/commands/kick.cpp \
	  server/commands/list.cpp \
	  server/commands/mode.cpp \
	  server/commands/part.cpp \
	  server/commands/registerUser.cpp \
	  server/commands/topic.cpp \
	  replies/Replies.cpp \

OBJ := $(SRC:.cpp=.o)

# Executable name
NAME := ircserv

# Default rule
all: $(NAME)

# Linking rule: creating the executable from object files
$(NAME): $(OBJ)
	$(CPP) $(CPP_FLAGS) $(OBJ) -o $(NAME)

# Compiling .cpp files into .o files
%.o: %.cpp
	$(CPP) $(CPP_FLAGS) -c $< -o $@

# Clean up object files
clean:
	rm -f $(OBJ)

# Clean up object files and the executable
fclean: clean
	rm -f $(NAME)

# Rebuild the project from scratch
re: fclean all

val: re
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./$(NAME) 6667 1234

# .PHONY rule: make targets that aren't filenames
.PHONY: all clean fclean re
