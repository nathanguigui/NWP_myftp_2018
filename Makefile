##
## EPITECH PROJECT, 2019
## NWP_myftp_2018
## File description:
## Makefile
##

NAME	= myftp

CC	= gcc

RM	= rm -f

SRCS	= 	src/my_ftp.c \
			src/client.c \
			src/server.c \
			src/commands.c \
			src/commands2.c \
			src/commands3.c \
			src/str_to_tab.c \
			src/utils.c

OBJS	= $(SRCS:.c=.o)

CFLAGS = -I ./inc/
CFLAGS += -Wall -Wextra

all: $(NAME)

$(NAME): $(OBJS)
	 $(CC) $(OBJS) -g3 -o $(NAME) $(LDFLAGS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
