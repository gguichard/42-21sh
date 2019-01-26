# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/01/03 13:31:22 by gguichar          #+#    #+#              #
#    Updated: 2019/01/27 00:39:35 by gguichar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	21sh

SRC_DIR	=	src
SRC		=	main.c \
			vars/env.c vars/vars.c vars/shell_vars.c \
			term/setup.c term/prompt.c \
			input/input.c input/read.c input/multiline.c input/cmdline.c \
			input/esc_sequence.c input/history.c input/history_hooks.c \
			input/insert_mode.c input/insert_mode_ac.c input/visual_mode.c \
			input/cursor_hooks.c input/cursor_hooks2.c input/del_hooks.c \
			input/misc_hooks.c input/utils.c \
			lexer/tokens.c lexer/grammar.c \
			exec/binaries.c exec/execute.c exec/single.c \
			exec/redirections.c exec/redir_output.c exec/redir_input.c \
			exec/redir_file.c exec/heredoc.c exec/pipeline.c exec/utils.c \
			parser/split_cmd_token.c parser/str_cmd_inf.c \
			parser/token_inf.c parser/apply_escape.c \
			autocomplete/autocomplete.c autocomplete/utils.c \
			autocomplete/autocomplete_cmdline.c \
			misc/error.c misc/check_path.c misc/hash_table.c \
			misc/convert_path_to_tab.c misc/utils.c \
			builtins/change_dir.c builtins/utils.c \
			join_token_cmd.c cmd_inf.c redirect_inf.c \
			expand_vars.c

OBJ_DIR	=	.obj
OBJ		=	$(SRC:.c=.o)
DEP		=	$(OBJ:.o=.d)

INC_DIR	=	includes

CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror -I libft/includes -I $(INC_DIR)

LIBFT	=	libft/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(addprefix $(OBJ_DIR)/,$(OBJ))
	$(CC) $(CFLAGS) -ltermcap -o $@ $^

$(LIBFT):
	$(MAKE) -C libft

-include $(addprefix $(OBJ_DIR)/,$(DEP))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(addprefix $(INC_DIR)/,$(INC)) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

$(OBJ_DIR):
	/bin/mkdir $@
	/bin/mkdir $@/vars
	/bin/mkdir $@/term
	/bin/mkdir $@/input
	/bin/mkdir $@/lexer
	/bin/mkdir $@/parser
	/bin/mkdir $@/exec
	/bin/mkdir $@/autocomplete
	/bin/mkdir $@/misc
	/bin/mkdir $@/builtins

clean:
	$(MAKE) -C libft clean
	/bin/rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C libft fclean
	/bin/rm -f $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re
