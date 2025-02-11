CC				= cc
CFLAGS			= -Wall -Wextra -Werror -pthread

NAME			= philo

MANDATORY_DIR 		= philo
BONUS_DIR			= philo_bonus

SRC				= $(wildcard $(MANDATORY_DIR)/*.c)
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