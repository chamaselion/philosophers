CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRCS = philo_main.c philo_utis.c philo_get_args.c philo_routine_utis.c philo_routines.c philo_get_args2.c
OBJS = $(SRCS:.c=.o)
NAME = philo

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

objects/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re