# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gnicolo <gnicolo@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/24 13:51:10 by gnicolo           #+#    #+#              #
#    Updated: 2026/02/01 by gnicolo                  ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors
GREEN = \033[0;32m
RED = \033[0;31m
BLUE = \033[0;34m
RESET = \033[0m

# Compiler
CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
INCLUDES = -I./include

# Directories
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

# Program name
NAME = philo

# Source files
SRCS = $(SRC_DIR)/main.c \
       $(SRC_DIR)/utils.c \
       $(SRC_DIR)/init_philo.c \
       $(SRC_DIR)/routine.c \
       $(SRC_DIR)/print_state.c \
       $(SRC_DIR)/monitor.c

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Default rule
all: $(NAME)

# Create obj directory
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "$(BLUE)Creating object directory...$(RESET)"

# Linking
$(NAME): $(OBJ_DIR) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)✓ Philosophers compiled successfully!$(RESET)"

# Compilation rule
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@echo "$(GREEN)Compiled: $<$(RESET)"

# Clean object files
clean:
	@rm -rf $(OBJ_DIR)
	@echo "$(RED)✗ Object files removed$(RESET)"

# Clean everything
fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)✗ Executable removed$(RESET)"

# Recompile everything
re: fclean all

# Valgrind - Memory leak detection
val: $(NAME)
	@echo "$(BLUE)Running Valgrind...$(RESET)"
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
	         --verbose ./$(NAME) 5 800 200 200

# Helgrind - Thread error detection
hel: $(NAME)
	@echo "$(BLUE)Running Helgrind...$(RESET)"
	valgrind --tool=helgrind --log-file=helgrind.log ./$(NAME) 5 800 200 200
	@echo "$(GREEN)✓ Helgrind log saved to helgrind.log$(RESET)"

.PHONY: all clean fclean re val hel