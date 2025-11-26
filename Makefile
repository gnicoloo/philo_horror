# Compiler
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Program name
NAME = philo

# Source files
SRC = main.c \
      utils.c \
      init_philo.c \
      routine.c \
      print_state.c\
	  monitor.c

# Object files
OBJ = $(SRC:.c=.o)

# Default rule
all: $(NAME)

# Linking
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

# Compilation rule
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object files
clean:
	rm -f $(OBJ)

# Clean everything
fclean: clean
	rm -f $(NAME)

# Recompile everything
re: fclean all

.PHONY: all clean fclean re
# Compiler