# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/01/03 13:31:22 by gguichar          #+#    #+#              #
#    Updated: 2019/01/03 17:14:54 by gguichar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= 	21sh

SRC		= 	main.c env.c vars.c error.c check_path.c
SRC_DIR	= 	src

OBJ		= 	$(SRC:.c=.o)
OBJ_DIR	= 	.obj

HEADERS	=	includes/shell.h includes/error.h includes/check_path.h

CC		= 	gcc
CFLAGS	= 	-Wall -Wextra -Werror -I libft/includes -I includes

LIBFT	= 	libft/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(addprefix $(OBJ_DIR)/,$(OBJ))
	$(CC) $(CFLAGS) -o $@ $^

$(LIBFT):
	$(MAKE) -C libft

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	/bin/mkdir $@

clean:
	$(MAKE) -C libft clean
	/bin/rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C libft fclean
	/bin/rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
