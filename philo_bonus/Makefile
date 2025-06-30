CC				= cc
CFLAGS			= -Wall -Wextra -Werror -pthread

NAME			= philo_bonus

SRC				= main_bonus.c \
				  scene_bonus.c \
				  routine_bonus.c \
				  routine_func_bonus.c \
				  protected_vars.c \
				  input_bonus.c \
				  input_checkers_bonus.c \
				  time.c
OBJ				= $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re