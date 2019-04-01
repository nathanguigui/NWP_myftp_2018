NAME	= myftp

CC	= gcc

RM	= rm -f

SRCS	= ./src/my_ftp.c 

OBJS	= $(SRCS:.c=.o)

CFLAGS = -I ./inc/
CFLAGS += -Wall -Wextra

all: $(NAME)

$(NAME): $(OBJS)
	 $(CC) $(OBJS) -o $(NAME) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
