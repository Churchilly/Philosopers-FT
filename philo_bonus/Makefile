CC				= cc
CFLAGS			= -Wall -Wextra -Werror -pthread

NAME			= philo_bonus

SRC				= main_bonus.c \
				  scene_bonus.c \
				  scene_actors_bonus.c \
				  routine_bonus.c \
				  routine_actions_bonus.c \
				  protected_vars_bonus.c \
				  logging_bonus.c \
				  scene_cleanup_bonus.c \
				  input_bonus.c \
				  input_checkers_bonus.c \
				  time_bonus.c
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