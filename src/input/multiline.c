/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiline.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/19 11:55:50 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/24 10:14:24 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "str_cmd_inf.h"
#include "token_inf.h"

int	handle_multiline_eot(t_term *term)
{
	term->prompt = PROMPT_DEF;
	ft_strdel(&(term->multiline));
	ft_dprintf(2, "%s: unexpected end of file\n", ERR_PREFIX);
	return (1);
}

int	is_multiline(t_term *term, t_str_cmd_inf *str_cmd_inf, t_list *all_sub_cmd)
{
	t_list	*cur_sub_cmd;

	if (str_cmd_inf->is_in_quote)
		term->prompt = PROMPT_QUOTE;
	else if (str_cmd_inf->is_in_doublequote)
		term->prompt = PROMPT_DQUOTE;
	else if (str_cmd_inf->is_in_var_bracket)
		term->prompt = PROMPT_BRACKET;
	else if (scmd_cur_char_is_escaped(str_cmd_inf))
		term->prompt = PROMPT_ESCAPED;
	else
	{
		cur_sub_cmd = all_sub_cmd;
		while (cur_sub_cmd->next != NULL)
			cur_sub_cmd = cur_sub_cmd->next;
		if (((t_token_inf *)cur_sub_cmd->content)->type == TK_OPE)
		{
			term->prompt = PROMPT_OPE;
			return (1);
		}
		term->prompt = PROMPT_DEF;
		return (0);
	}
	return (1);
}
