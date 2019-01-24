/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 09:14:11 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/24 14:05:48 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "vars.h"
#include "shell.h"
#include "str_cmd_inf.h"

static int		str_good_replace(char **str, size_t pos, size_t len,
		const char *replacement)
{
	size_t	str_len;
	size_t	replacement_len;
	char	*new_str;

	str_len = ft_strlen(*str);
	replacement_len = ft_strlen(replacement);
	if ((new_str = (char*)malloc(str_len - len + replacement_len + 1)) == NULL)
		return (0);
	new_str[0] = '\0';
	ft_strncat(new_str, *str, pos);
	ft_strcat(new_str, replacement);
	ft_strcat(new_str, *str + pos + len);
	free(*str);
	*str = new_str;
	return (1);
}

static size_t	get_var_name_len(const char *str)
{
	size_t	var_len;

	var_len = 0;
	if (*str == '?' || ft_isdigit(*str))
		var_len = 1;
	else
	{
		while (ft_isalnum(*str) || *str == '_')
		{
			++var_len;
			++str;
		}
	}
	return (var_len);
}

static int		replace_var_by_value(t_str_cmd_inf *scmd, char **new_str,
		size_t var_len, t_shell *shell)
{
	int		is_in_brackets;
	int		err_ret;
	char	*var_name;
	char	*var_value;

	is_in_brackets = (scmd->str[scmd->pos + 1] == '{');
	if ((var_name = ft_strsub(scmd->str, scmd->pos + (is_in_brackets ? 2 : 1),
					var_len)) == NULL)
		return (0);
	var_value = get_shell_var(shell, var_name);
	err_ret = str_good_replace(new_str, scmd->pos,
			var_len + (is_in_brackets ? 3 : 1), var_value);
	scmd->str = *new_str;
	scmd->pos += ft_strlen(var_value);
	free(var_value);
	free(var_name);
	return (err_ret);
}

char			*expand_vars(const char *str, t_shell *shell)
{
	t_str_cmd_inf	scmd;
	char			*new_str;
	size_t			old_pos;
	size_t			var_in_bracket_len;
	int				old_is_in_var_bracket;

	if ((new_str = ft_strdup(str)) == NULL)
		return (NULL);
	scmd_init(&scmd, new_str);
	old_pos = 0;
	old_is_in_var_bracket = 0;
	while (1)
	{
		if (scmd.is_in_var_bracket != old_is_in_var_bracket)
		{
			if (scmd.is_in_var_bracket)
				old_pos = scmd.pos;
			else
			{
				var_in_bracket_len = scmd.pos - 1 - old_pos;
				scmd.pos = old_pos - 2;
				if (!replace_var_by_value(&scmd, &new_str, var_in_bracket_len,
							shell))
				{
					scmd_delete(scmd.sub_var_bracket);
					return (NULL);
				}
			}
		}
		else if (!scmd.is_in_quote && !scmd.is_in_var_bracket)
		{
			if (scmd.str[scmd.pos] == '$' && scmd.str[scmd.pos + 1] != '{'
					&& !scmd_cur_char_is_escaped(&scmd))
			{
				if (!replace_var_by_value(&scmd, &new_str,
							get_var_name_len(scmd.str + scmd.pos + 1), shell))
				{
					scmd_delete(scmd.sub_var_bracket);
					return (NULL);
				}
			}
		}
		old_is_in_var_bracket = scmd.is_in_var_bracket;
		if (scmd.str[scmd.pos] == '\0')
			break ;
		scmd_move_to_next_char(&scmd);
	}
	scmd_delete(scmd.sub_var_bracket);
	return (new_str);
}
