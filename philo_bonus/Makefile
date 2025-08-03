CFLAGS = -Wall -Wextra -Werror #-g3 -fsanitize=thread 
CC = cc
SRCS =	initialization_bonus.c utils_bonus.c \
		main_bonus.c philosophers_bonus.c  \
		stop_simulation_bonus.c free_data.c
OBJS = $(SRCS:.c=.o)
NAME = philo_bonus

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.SECONDARY: $(OBJS) $(SRCS)
