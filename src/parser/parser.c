/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 14:26:58 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/12 21:39:03 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "libft.h"
#include "parser.h"
#include "printf.h"

void	next_token(t_list **tokens)
{
	if (*tokens == NULL)
		return ;
	*tokens = (*tokens)->next;
}

int		accept_token(t_list **tokens, t_token_type type)
{
	if (*tokens != NULL && ((t_token_inf *)((*tokens)->content))->type == type)
	{
		next_token(tokens);
		return (1);
	}
	return (0);
}

int		expect_token(t_list **tokens, t_token_type type)
{
	char	*curr;

	if (accept_token(tokens, type))
		return (1);
	curr = "newline";
	if (*tokens != NULL)
		curr = ((t_token_inf *)(*tokens)->content)->token;
	ft_dprintf(2, "%s: syntax error near unexpected token `%s'\n"
			, ERR_PREFIX, curr);
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
	if (accept_token(tokens, TK_CMD_SEP))
		parse_command(tokens);
	else if (*tokens != NULL)
	{
		ft_dprintf(2, "%s: syntax error near unexpected token `newline'\n"
				, ERR_PREFIX);
	}
}

void	split_commands(t_list *tokens)
{
	parse_command(&tokens);
}
