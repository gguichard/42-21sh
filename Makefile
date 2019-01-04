# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/01/03 13:31:22 by gguichar          #+#    #+#              #
#    Updated: 2019/01/04 12:20:19 by gguichar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= 	21sh

SRC		= 	main.c env.c vars.c shell_vars.c \
			term.c input.c cmdline.c esc_seq.c \
			cursor_hooks.c error.c check_path.c utils.c
SRC_DIR	= 	src

OBJ		= 	$(SRC:.c=.o)
OBJ_DIR	= 	.obj

INC_DIR	=	includes
INC		=	shell.h vars.h error.h check_path.h input.h utils.h

CC		= 	gcc
CFLAGS	= 	-Wall -Wextra -Werror -I libft/includes -I $(INC_DIR)

LIBFT	= 	libft/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(addprefix $(OBJ_DIR)/,$(OBJ))
	$(CC) $(CFLAGS) -ltermcap -o $@ $^

$(LIBFT):
	$(MAKE) -C libft

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(addprefix $(INC_DIR)/,$(INC)) | $(OBJ_DIR)
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
