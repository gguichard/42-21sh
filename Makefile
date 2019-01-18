# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/01/03 13:31:22 by gguichar          #+#    #+#              #
#    Updated: 2019/01/18 13:14:34 by gguichar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	21sh

SRC		=	main.c env.c vars.c shell_vars.c prompt.c \
			input/term.c input/input.c input/cmdline.c \
			input/esc_sequence.c input/history.c input/history_hooks.c \
			input/insert_mode.c input/insert_mode_ac.c input/visual_mode.c \
			input/cursor_hooks.c input/cursor_hooks2.c input/del_hooks.c \
			input/utils.c \
			lexer/lexer.c lexer/parsing.c parser/execute.c \
			error.c check_path.c hash_table.c convert_path_to_tab.c utils.c \
			autocomplete/autocomplete.c autocomplete/utils.c \
			check_enclosing_char_cmd.c split_cmd_token.c str_cmd_inf.c \
			token_inf.c apply_escape.c
SRC_DIR	=	src

OBJ		=	$(SRC:.c=.o)
OBJ_DIR	=	.obj

INC_DIR	=	includes
INC		=	shell.h vars.h error.h check_path.h input.h utils.h \
			hash_table.h convert_path_to_tab.h autocomplete.h str_cmd_inf.h \
			split_cmd_token.h check_enclosing_char_cmd.h lexer.h token_inf.h \
			apply_escape.h execute.h

CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror -I libft/includes -I $(INC_DIR)

LIBFT	=	libft/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(addprefix $(OBJ_DIR)/,$(OBJ))
	$(CC) $(CFLAGS) -ltermcap -o $@ $^

$(LIBFT):
	$(MAKE) -C libft

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(addprefix $(INC_DIR)/,$(INC)) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	/bin/mkdir $@
	/bin/mkdir $@/input
	/bin/mkdir $@/autocomplete
	/bin/mkdir $@/lexer
	/bin/mkdir $@/parser

clean:
	$(MAKE) -C libft clean
	/bin/rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C libft fclean
	/bin/rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
