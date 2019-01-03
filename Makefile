# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/01/03 13:31:22 by gguichar          #+#    #+#              #
#    Updated: 2019/01/03 16:12:40 by fwerner          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	21sh

SRC_DIR	=	src
SRC_DEF	=	main.c \
			check_path.c \
			error.c
SRC		=	$(addprefix $(SRC_DIR)/,$(SRC_DEF))
OBJ		=	$(SRC:.c=.o)

INC_DIR	=	includes
INC_DEF	=	shell.h \
			check_path.h \
			error.h
INC		=	$(addprefix $(INC_DIR)/,$(INC_DEF))

CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror

LIBFT	=	libft/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(LIBS) -o $@ $^

$(LIBFT):
	$(MAKE) -C libft

%.o: %.c $(INC)
	$(CC) $(CFLAGS) -I libft/includes -I includes -o $@ -c $<

clean:
	$(MAKE) -C libft clean
	/bin/rm -f $(OBJ)

fclean: clean
	$(MAKE) -C libft fclean
	/bin/rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
