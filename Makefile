# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lwoiton <lwoiton@student.42prague.com>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/08 11:27:34 by lwoiton           #+#    #+#              #
#    Updated: 2024/08/06 22:06:39 by lwoiton          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Target executables
NAME  = webserv

# Compiler Settings
COMP  = g++
FLAGS = -Werror -Wextra -Wall -std=c++98
INCL  = -Iincl

# Directories
SRC_DIR = srcs
OBJ_DIR = obj

# Files and extensions
SRCS = $(shell find $(SRC_DIR) -type f -name '*.cpp')
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

# Default rule to build the main executable
all: $(NAME)

# Link the main program
$(NAME): $(OBJS)
	$(COMP) $(FLAGS) $^ -o $@

# Compile and generate dependency info for main program
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(COMP) $(FLAGS) $(INCL) -MMD -c $< -o $@

# Include dependencies for main program
-include $(DEPS)

# Debug configuration
debug: FLAGS += -g
debug: re

# Clean generated files
clean:
	rm -rf $(OBJ_DIR)

# Clean everything
fclean: clean
	rm -f $(NAME)

# Rebuild everything
re: fclean all

# Phony targets
.PHONY: all clean fclean re debug test
