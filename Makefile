CC = gcc
CFLAGS = -Wall -Wextra -Werror
SRCS = philo_main.c philo_utis.c philo_get_args.c
OBJS = $(addprefix objects/, $(SRCS:.c=.o))
NAME = philo

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

objects/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f objects/*.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re