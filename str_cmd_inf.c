/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_cmd_inf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 12:15:36 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/09 14:45:00 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "str_cmd_inf.h"

void	scmd_init(t_str_cmd_inf *str_cmd_inf, char *str)
{
	str_cmd_inf->is_in_quote = 0;
	str_cmd_inf->is_in_doublequote = 0;
	str_cmd_inf->is_in_var_bracket = 0;
	str_cmd_inf->pos = 0;
	str_cmd_inf->str = str;
}

int		scmd_cur_char_is_in_nothing(t_str_cmd_inf *str_cmd_inf)
{
	return (!str_cmd_inf->is_in_quote
			&& !str_cmd_inf->is_in_doublequote
			&& !str_cmd_inf->is_in_var_bracket);
}

int		scmd_cur_char_is_escaped(t_str_cmd_inf *str_cmd_inf)
{
	if (str_cmd_inf->pos == 0 || str_cmd_inf->is_in_quote)
		return (0);
	return (str_cmd_inf->str[str_cmd_inf->pos - 1] == '\\');
}

int		scmd_move_to_next_char(t_str_cmd_inf *str_cmd_inf)
{
	if (str_cmd_inf->str[str_cmd_inf->pos] == '\0')
		return (0);
	++(str_cmd_inf->pos);
	if (scmd_cur_char_is_escaped(str_cmd_inf))
		return (1);
	if (str_cmd_inf->is_in_quote)
	{
		if (str_cmd_inf->str[str_cmd_inf->pos] == '\'')
			str_cmd_inf->is_in_quote = 0;
	}
	else if (str_cmd_inf->is_in_var_bracket)
	{
		if (str_cmd_inf->str[str_cmd_inf->pos] == '}')
			str_cmd_inf->is_in_var_bracket = 0;
	}
	else
	{
		if (str_cmd_inf->str[str_cmd_inf->pos] == '\''
				&& !str_cmd_inf->is_in_doublequote)
			str_cmd_inf->is_in_quote = 1;
		else if (str_cmd_inf->str[str_cmd_inf->pos] == '\"')
			str_cmd_inf->is_in_doublequote = !str_cmd_inf->is_in_doublequote;
		else if (str_cmd_inf->str[str_cmd_inf->pos] == '{'
				&& str_cmd_inf->pos > 0
				&& str_cmd_inf->str[str_cmd_inf->pos - 1] == '$')
			str_cmd_inf->is_in_var_bracket = 1;
	}
	return (1);
}
