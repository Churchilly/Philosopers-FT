CC				= cc
CFLAGS			= -Wall -Wextra -Werror -pthread

NAME			= philo_bonus

SRC				= $(wildcard *.c)
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