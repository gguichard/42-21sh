/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_enclosing_char_cmd.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 08:29:46 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/11 12:54:27 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "check_enclosing_char_cmd.h"

static size_t			count_var_len(const char *str)
{
	size_t	len;

	len = 0;
	if (str[len] == '?')
		len = 1;
	else if (ft_isdigit(str[len]))
	{
		while (ft_isdigit(str[len]))
			++len;
	}
	else
	{
		while (ft_isalnum(str[len]) || str[len] == '_')
			++len;
	}
	return (len);
}

t_str_cmd_err			check_enclosing_char_cmd(t_str_cmd_inf *str_cmd_inf)
{
	size_t			var_len;
	t_str_cmd_err	last_err;

	last_err = SCMDERR_NOERROR;
	while (str_cmd_inf->str[str_cmd_inf->pos] != '\0')
	{
		if (str_cmd_inf->is_in_var_bracket)
		{
			str_cmd_inf->pos += (var_len = count_var_len(str_cmd_inf->str
					+ str_cmd_inf->pos));
			if (var_len == 0 || (str_cmd_inf->str[str_cmd_inf->pos] != '}'
						&& str_cmd_inf->str[str_cmd_inf->pos] != '\0'))
			{
				last_err = SCMDERR_BADCHAR;
				break ;
			}
		}
		scmd_move_to_next_char(str_cmd_inf);
	}
	if (last_err == SCMDERR_NOERROR && (scmd_cur_char_is_escaped(str_cmd_inf)
				|| str_cmd_inf->is_in_quote || str_cmd_inf->is_in_doublequote
				|| str_cmd_inf->is_in_var_bracket))
		last_err = SCMDERR_ENDTOOSOON;
	str_cmd_inf->pos = 0;
	return (last_err);
}
