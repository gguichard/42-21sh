/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiline.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/19 11:55:50 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/20 14:18:49 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "str_cmd_inf.h"

int	handle_multiline_eot(t_term *term)
{
	term->prompt = PROMPT_DEF;
	ft_strdel(&(term->multiline));
	ft_dprintf(2, "%s: unexpected end of file\n", ERR_PREFIX);
	return (1);
}

int	is_multiline(t_term *term, t_str_cmd_inf *str_cmd_inf)
{
	if (str_cmd_inf->is_in_quote)
	{
		term->prompt = PROMPT_QUOTE;
		return (1);
	}
	if (str_cmd_inf->is_in_doublequote)
	{
		term->prompt = PROMPT_DQUOTE;
		return (1);
	}
	if (scmd_cur_char_is_escaped(str_cmd_inf))
	{
		term->prompt = PROMPT_ESCAPED;
		return (1);
	}
	term->prompt = PROMPT_DEF;
	return (0);
}
