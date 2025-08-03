CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 -fsanitize=thread 
SRCS =	initialization.c  threads.c  utils.c main.c philosophers.c stop_simulation.c get_time.c
OBJS = $(SRCS:.c=.o)
NAME = philo

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)


fclean: clean
	rm -rf $(NAME)

re: fclean all

.SECONDARY: $(OBJS) $(SRCS)
