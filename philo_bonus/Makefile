CC = cc
CFLAGS = -Wall -Wextra -Werror #-g3 -fsanitize=thread 
SRCS =	
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
