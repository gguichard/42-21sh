/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 14:26:58 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/22 11:02:49 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "libft.h"
#include "lexer.h"

int	accept_str_opt(t_list **curr)
{
	int	expect_word;

	if (*curr != NULL
			&& ((t_token_inf *)((*curr)->content))->type == TK_STR_OPT)
	{
		expect_word = ft_strequ("&"
				, ((t_token_inf *)((*curr)->content))->token);
		next_token(curr);
		return (!expect_word);
	}
	return (0);
}

int	parse_operator(t_list **curr)
{
	int	ret;

	ret = 0;
	if (accept_token(curr, TK_OPE))
		ret = 1;
	else if (accept_token(curr, TK_NUM_OPT))
		ret = !expect_token(curr, TK_OPE) ? -1 : 1;
	return (ret);
}

int	parse_command_part(t_list **curr)
{
	int	ret;

	while (accept_token(curr, TK_WORD))
		continue ;
	ret = parse_operator(curr);
	if (ret < 0)
		return (-1);
	if (ret > 0 && !accept_str_opt(curr))
		return (!expect_token(curr, TK_WORD) ? -1 : 1);
	return (ret);
}

int	parse_commands(t_list *tokens)
{
	t_list	*curr;
	int		ret;

	curr = tokens;
	if (!expect_token(&curr, TK_WORD))
		return (0);
	ret = 0;
	while ((ret = parse_command_part(&curr)) > 0)
		continue ;
	if (ret < 0)
		return (0);
	if (accept_token(&curr, TK_CMD_SEP) && curr != NULL)
		return (parse_commands(curr));
	if (curr != NULL)
	{
		ft_dprintf(2, "%s: syntax error near unexpected token `newline'\n"
				, ERR_PREFIX);
		return (0);
	}
	return (1);
}
