/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 09:43:25 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/10 14:41:52 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

#include "str_cmd_inf.h"

typedef enum		e_tokret
{
	TOKR_DELIMIT,
	TOKR_DELIMIT_DISCARD,
	TOKR_APPEND
}					t_tokret;

typedef enum		e_toktype
{
	TOKT_WORD,
	TOKT_ASSIGNMENT_WORD,
	TOKT_NAME,
	TOKT_NEWLINE,
	TOKT_IO_NUMBER,
	TOKT_DLESS,
	TOKT_DGREAT,
	TOKT_LESSAND,
	TOKT_GREATAND,
	TOKT_LESSGREAT,
	TOKT_DLESSDASH
}					t_toktype;

typedef struct		s_token
{
	char			*content;
	t_toktype		type;
	struct s_token	*next;
	struct s_token	*prev;
}					t_token;

typedef struct		s_parser
{
	const char		*tok_start;
	size_t			tok_len;
	int				can_be_operator;
}					t_parser;

int					create_token(t_token **lst, t_parser *parser);
t_token				*free_tokens(t_token **lst);

t_token				*tokenize(const char *str);

#endif
