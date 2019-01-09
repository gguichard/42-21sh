/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_enclosing_char_cmd.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 08:29:46 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/09 11:14:52 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "check_enclosing_char_cmd.h"

static size_t			count_var_len(char *str)
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

static t_str_cmd_err	check_quote_or_var_bracket_at(char **pos,
		t_str_cmd_inf *str_cmd_inf)
{
	size_t	var_len;

	if (str_cmd_inf->is_in_quote)
	{
		while (**pos != '\0' && **pos != '\'')
			++(*pos);
		if (**pos == '\0')
			return (SCMDERR_ENDTOOSOON);
		str_cmd_inf->is_in_quote = 0;
	}
	else if (str_cmd_inf->is_in_var_bracket)
	{
		if ((var_len = count_var_len(*pos)) == 0)
			return (SCMDERR_BADCHAR);
		*pos += var_len;
		if (**pos == '\0')
			return (SCMDERR_ENDTOOSOON);
		else if (**pos != '}')
			return (SCMDERR_BADCHAR);
		str_cmd_inf->is_in_var_bracket = 0;
	}
	return (SCMDERR_NOERROR);
}

static int				check_not_inside_quote_or_var_bracket_at(char **pos,
		t_str_cmd_inf *str_cmd_inf)
{
	if (ft_strchr("\'\"$", **pos) != NULL)
	{
		if (**pos == '\'')
		{
			if (!str_cmd_inf->is_in_doublequote)
			{
				str_cmd_inf->is_in_quote = 1;
				return (0);
			}
		}
		else if (**pos == '\"')
		{
			str_cmd_inf->is_in_doublequote = !str_cmd_inf->is_in_doublequote;
		}
		else if (**pos == '$')
		{
			if ((*pos)[1] == '{')
			{
				++(*pos);
				str_cmd_inf->is_in_var_bracket = 1;
				return (0);
			}
		}
	}
	return (1);
}

static t_str_cmd_err	loop_check_while_dquote_or_nothing(char **pos,
		t_str_cmd_inf *str_cmd_inf)
{
	while (**pos != '\0')
	{
		if (**pos == '\\')
		{
			++(*pos);
			if (**pos == '\0')
				return (SCMDERR_ENDTOOSOON);
		}
		else if (!check_not_inside_quote_or_var_bracket_at(pos,
					str_cmd_inf))
			break ;
		if (**pos != '\0')
			++(*pos);
	}
	return (SCMDERR_NOERROR);
}

t_str_cmd_err			check_enclosing_char_cmd(t_str_cmd_inf *str_cmd_inf)
{
	t_str_cmd_err	last_err;
	char			*cur_pos;

	cur_pos = str_cmd_inf->str + str_cmd_inf->pos;
	while (*cur_pos != '\0')
	{
		if (str_cmd_inf->is_in_quote || str_cmd_inf->is_in_var_bracket)
		{
			if ((last_err = check_quote_or_var_bracket_at(&cur_pos,
							str_cmd_inf)) != SCMDERR_NOERROR)
				return (last_err);
		}
		else
		{
			if ((last_err = loop_check_while_dquote_or_nothing(&cur_pos,
							str_cmd_inf)) != SCMDERR_NOERROR)
				return (last_err);
		}
		if (*cur_pos != '\0')
			++cur_pos;
	}
	if (str_cmd_inf->is_in_quote || str_cmd_inf->is_in_doublequote
			|| str_cmd_inf->is_in_var_bracket)
		return (SCMDERR_ENDTOOSOON);
	return (SCMDERR_NOERROR);
}
