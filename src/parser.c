/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 10:04:08 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/10 15:39:40 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "parser.h"

static t_toktype	find_corresponding_type(const char *content)
{
	(void)content;
	return (TOKT_WORD);
}

t_token				*free_tokens(t_token **lst)
{
	t_token	*curr;
	t_token	*next;

	curr = *lst;
	while (curr != NULL)
	{
		next = curr->next;
		free(curr->content);
		curr = next;
	}
	*lst = NULL;
	return (NULL);
}

int					create_token(t_token **lst, t_parser *parser)
{
	t_token	*token;
	t_token	*curr;

	token = (t_token *)malloc(sizeof(t_token));
	if (token != NULL)
	{
		token->content = ft_strsub(parser->tok_start, 0, parser->tok_len);
		if (token->content == NULL)
			ft_memdel((void **)&token);
		else
		{
			curr = *lst;
			if (curr != NULL)
			{
				while (curr->next != NULL)
					curr = curr->next;
			}
			token->prev = curr;
			token->next = NULL;
			token->type = find_corresponding_type(token->content);
			if (curr == NULL)
				*lst = token;
			else
				curr->next = token;
		}
	}
	return (token != NULL);
}

int					is_tok_operator(const char *str, size_t len)
{
	if (ft_strnequ(str, "<<", len) || ft_strnequ(str, ">>", len)
			|| ft_strnequ(str, "<&", len) || ft_strnequ(str, ">&", len)
			|| ft_strnequ(str, "<>", len) || ft_strnequ(str, "<<-", len))
		return (1);
	else
		return (0);
}

t_tokret			parse_token(t_parser *parser, t_str_cmd_inf *scmd)
{
	int		is_quoted;
	char	c;

	is_quoted = scmd->is_in_quote || scmd->is_in_doublequote;
	if (parser->can_be_operator)
	{
		if (!is_quoted
				&& is_tok_operator(parser->tok_start, parser->tok_len + 1))
			return (TOKR_APPEND);
	}
	c = (scmd->str)[scmd->pos - 1];
	if (!is_quoted && is_tok_operator(&c, 1))
	{
		parser->can_be_operator = 1;
		return (TOKR_DELIMIT);
	}
	if (!is_quoted && c == ' ')
		return (TOKR_DELIMIT_DISCARD);
	if (!(parser->can_be_operator))
		return (TOKR_APPEND);
	parser->can_be_operator = 0;
	return (TOKR_DELIMIT);
}

t_token				*tokenize(const char *str)
{
	t_token			*lst;
	t_str_cmd_inf	scmd;
	t_tokret		ret;
	t_parser		parser;

	lst = NULL;
	scmd_init(&scmd, str);
	ft_memset(&parser, 0, sizeof(t_parser));
	while (scmd_move_to_next_char(&scmd))
	{
		if (parser.tok_start == NULL)
		{
			parser.tok_start = str;
			parser.tok_len = 0;
		}
		ret = parse_token(&parser, &scmd);
		if (ret == TOKR_APPEND)
			(parser.tok_len)++;
		else
		{
			if (!create_token(&lst, &parser))
				return (free_tokens(&lst));
			parser.tok_start = (ret == TOKR_DELIMIT) ? NULL : (str + 1);
			parser.tok_len = 0;
		}
		str++;
	}
	if (!create_token(&lst, &parser))
		return (free_tokens(&lst));
	return (lst);
}
