/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiline.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/19 11:55:50 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/27 17:25:15 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "str_cmd_inf.h"
#include "token_inf.h"

int			is_command_complete(t_str_cmd_inf *scmd_inf, t_list *token_lst)
{
	t_list	*cur_token;

	if (scmd_inf->is_in_quote
			|| scmd_inf->is_in_doublequote
			|| scmd_inf->is_in_var_bracket
			|| scmd_cur_char_is_escaped(scmd_inf))
		return (0);
	else
	{
		cur_token = token_lst;
		while (cur_token->next != NULL)
			cur_token = cur_token->next;
		if (((t_token_inf *)cur_token->content)->type == TK_OPE)
			return (0);
	}
	return (1);
}

t_prompt	get_prompt_type(t_str_cmd_inf *scmd_inf)
{
	if (scmd_inf->is_in_quote)
		return (PROMPT_QUOTE);
	else if (scmd_inf->is_in_doublequote)
		return (PROMPT_DQUOTE);
	else if (scmd_inf->is_in_var_bracket)
		return (PROMPT_BRACKET);
	else if (scmd_cur_char_is_escaped(scmd_inf))
		return (PROMPT_ESCAPED);
	else
		return (PROMPT_OPE);
}
