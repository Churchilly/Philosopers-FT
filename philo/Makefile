NAME = philo

SRCS = main.c \
       input_checkers.c \
       input.c \
       logging.c \
       monitoring.c \
	   mutex.c \
       routine_actions.c \
       routine.c \
       scene_actors.c \
       scene_clear.c \
       scene_create.c \
       time.c

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread
RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.c philosophers.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
