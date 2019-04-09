NAME	= myftp

CC	= gcc

RM	= rm -f

SRCS	= 	src/my_ftp.c \
			src/commands.c \
			src/commands.2.c \
			src/utils.c

OBJS	= $(SRCS:.c=.o)

CFLAGS = -I ./inc/
CFLAGS += -Wall -Wextra -g3

all: $(NAME)

$(NAME): $(OBJS)
	 $(CC) $(OBJS) -g3 -o $(NAME) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
