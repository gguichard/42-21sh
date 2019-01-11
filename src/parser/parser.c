/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 14:26:58 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/11 18:25:01 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token_inf.h"
#include "printf.h"

void	next_token(t_list **tokens)
{
	if (*tokens == NULL)
		return ;
	*tokens = (*tokens)->next;
}

int		accept_token(t_list **tokens, t_token_type type)
{
	if (*tokens != NULL && (*tokens)->type == type)
	{
		next_token(tokens);
		return (1);
	}
	return (0);
}

int		expect_token(t_list **tokens, t_token_type type)
{
	if (accept_token(tokens, type))
		return (1);
	ft_printf("unexpected token\n");
	return (0);
}

void	parse_command(t_list **tokens)
{
	while (accept_token(tokens, TK_WORD))
	{
		if (accept_token(tokens, TK_NUM_OPT)
				&& (!expect_token(tokens, TK_OPE)
					|| !expect_token(tokens, TK_WORD)))
			return ;
		if (accept_token(tokens, TK_OPE) && !expect_token(tokens, TK_WORD))
			return ;
	}
}

void	split_commands(t_list *tokens)
{
	t_list		*start;
	t_list		*prev;
	t_token_inf	*inf;

	start = tokens;
	prev = NULL;
	while (tokens != NULL)
	{
		inf = (t_token_inf *)tokens->content;
		if (inf->type == TK_OPE && ft_strequ(inf->content, ";"))
		{
			if (prev != NULL)
				prev->next = NULL;
			parse_command(&tokens);
			start = tokens->next;
		}
		prev = tokens;
		tokens = tokens->next;
	}
}
