NAME		= pipex

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g

LIBFT		= libft/libft.a
LIBFT_DIR	= libft

SRCS		= src/pipex.c \
			  src/pipex_utils.c \
			  src/path_utils.c \
			  src/ft_split_quote.c \
			  src/ft_split_quote_utils.c \
			  src/free_utils.c

OBJS		= $(SRCS:.c=.o)

INCLUDES	= -Iincludes -Ilibft

all: $(LIBFT) $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
