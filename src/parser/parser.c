/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 14:26:58 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/13 13:52:50 by gguichar         ###   ########.fr       */
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

int		parse_operator(t_list **curr)
{
	if (accept_token(curr, TK_OPE))
		return (1);
	if (accept_token(curr, TK_NUM_OPT))
		return (!expect_token(curr, TK_OPE) ? -1 : 1);
	return (0);
}

void	parse_commands(t_list *tokens)
{
	t_list	*curr;
	int		ret;

	curr = tokens;
	if (!expect_token(&curr, TK_WORD))
		return ;
	while (accept_token(&curr, TK_WORD))
	{
		ret = parse_operator(&curr);
		if (ret < 0 || (ret > 0 && !expect_token(&curr, TK_WORD)))
			return ;
	}
	ret = parse_operator(&curr);
	if (ret < 0)
		return ;
	if (ret > 0 || (accept_token(&curr, TK_CMD_SEP) && curr != NULL))
		parse_commands(curr);
	else if (curr != NULL)
	{
		ft_dprintf(2, "%s: syntax error near unexpected token `newline'\n"
				, ERR_PREFIX);
	}
}
