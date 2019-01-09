/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_enclosing_char_cmd.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 08:29:46 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/09 09:54:23 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "check_enclosing_char_cmd.h"

static size_t	count_var_len(char *str)
{
	size_t		len;

	len = 0;
	if (str[len] == '?')
		len = 1;
	if (ft_isdigit(str[len]))
		while (ft_isdigit(str[len]))
			++len;
	else
		while (ft_isalnum(str[len]) || str[len] == '_')
			++len;
	return (len);
}

t_str_cmd_err	check_enclosing_char_cmd(t_str_cmd_inf *str_cmd_inf)
{
	char	*cur_pos;
	size_t	var_len;

	cur_pos = str_cmd_inf->str + str_cmd_inf->pos;
	while (*cur_pos != '\0')
	{
		if (str_cmd_inf->is_in_quote)
		{
			while (*cur_pos != '\0' && *cur_pos != '\'')
				++cur_pos;
			if (*cur_pos == '\0')
				return (SCMDERR_ENDTOOSOON);
			str_cmd_inf->is_in_quote = 0;
		}
		else if (str_cmd_inf->is_in_var_bracket)
		{
			if ((var_len = count_var_len(cur_pos)) == 0)
				return (SCMDERR_BADCHAR);
			cur_pos += var_len;
			if (*cur_pos == '\0')
				return (SCMDERR_ENDTOOSOON);
			else if (*cur_pos != '}')
				return (SCMDERR_BADCHAR);
			str_cmd_inf->is_in_var_bracket = 0;
		}
		else
		{
			while (*cur_pos != '\0')
			{
				if (*cur_pos == '\\')
				{
					++cur_pos;
					if (*cur_pos == '\0')
						return (SCMDERR_ENDTOOSOON);
				}
				else if (ft_strchr("\'\"$", *cur_pos) != NULL)
				{
					if (*cur_pos == '\'')
					{
						if (!str_cmd_inf->is_in_doublequote)
						{
							str_cmd_inf->is_in_quote = 1;
							break ;
						}
					}
					else if (*cur_pos == '\"')
					{
						str_cmd_inf->is_in_doublequote = !str_cmd_inf->is_in_doublequote;
					}
					else if (*cur_pos == '$')
					{
						if (cur_pos[1] == '{')
						{
							++cur_pos;
							str_cmd_inf->is_in_var_bracket = 1;
							break ;
						}
					}
				}
				if (*cur_pos != '\0')
					++cur_pos;
			}
		}
		if (*cur_pos != '\0')
			++cur_pos;
	}
	if (str_cmd_inf->is_in_quote || str_cmd_inf->is_in_doublequote || str_cmd_inf->is_in_var_bracket)
		return (SCMDERR_ENDTOOSOON);
	else
		return (SCMDERR_NOERROR);
}
