/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/17 15:22:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/17 15:24:02 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "libft.h"
#include "lexer.h"
#include "printf.h"

void	next_token(t_list **curr)
{
	if (*curr == NULL)
		return ;
	*curr = (*curr)->next;
}

int		accept_token(t_list **curr, t_token_type type)
{
	if (*curr != NULL && ((t_token_inf *)((*curr)->content))->type == type)
	{
		next_token(curr);
		return (1);
	}
	return (0);
}

int		expect_token(t_list **curr, t_token_type type)
{
	char	*token;

	if (accept_token(curr, type))
		return (1);
	token = "newline";
	if (*curr != NULL)
		token = ((t_token_inf *)(*curr)->content)->token;
	ft_dprintf(2, "%s: syntax error near unexpected token `%.*s'\n"
			, ERR_PREFIX, *curr != NULL ? 1 : 7, token);
	return (0);
}

int		parse_operator(t_list **curr)
{
	int	ret;

	ret = 0;
	if (accept_token(curr, TK_OPE))
		ret = 1;
	else if (accept_token(curr, TK_NUM_OPT))
		ret = !expect_token(curr, TK_OPE) ? -1 : 1;
	return (ret);
}
