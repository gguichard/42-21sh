/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_semicolon.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 16:51:29 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/08 17:01:32 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef enum	e_str_cmd_err
{
	SCMDERR_NOERROR = 0,
	SCMDERR_ENDTOOSOON,
	SCMDERR_BADCHAR
}				t_str_cmd_err;

typedef struct	s_str_cmd_inf
{
	int			is_in_quote;
	int			is_in_doublequote;
	int			is_in_var_bracket;
	size_t		pos;
	char		*str;
}				t_str_cmd_inf;

size_t	count_var_len(char *str)
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

char	*get_next_semicolon(t_str_cmd_inf *str_cmd_inf, t_str_cmd_err *err_code)
{
	char	*cur_pos;

	cur_pos = str_cmd_inf->str + str_cmd_inf->pos;
	while (*cur_pos != '\0' && *cur_pos != ';')
	{
		if (str_cmd_inf->is_in_quote)
		{
			while (*cur_pos != '\0' && *cur_pos != '\'')
				++cur_pos;
			if (err_code != NULL)
				err_code = (*cur_pos == '\0'
						? SCMDERR_ENDTOOSOON : SCMDERR_NOERROR);
			str_cmd_inf->is_in_quote = (*cur_pos == '\'' ? 0 : 1);
			if (*cur_pos == '\0')
				return (NULL);
		}
		else if (str_cmd_inf->is_in_var_bracket)
		{
			cur_pos += count_var_len(cur_pos);
			if (*cur_pos == '}')
				str_cmd_inf->is_in_var_bracket = 0;
			if (err_code != NULL)
			{
				if (*cur_pos == '}')
					err_code = SCMDERR_NOERROR;
				else if (*cur_pos == '\0')
					err_code = SCMDERR_ENDTOOSOON;
				else
					err_code = SCMDERR_BADCHAR;
			}
			if (*cur_pos == '\0' || *cur_pos != '}')
				return (NULL);
		}
		else
		{
			//TODO gerer $ / ${}
			while (*cur_pos != '\0')
			{
				if (*cur_pos == '\\')
					++cur_pos;
				else if (ft_strchr("\'\"$;{}", *cur_pos) != NULL)
				{
					if (str_cmd_inf->is_in_doublequote)
					{
						if (*cur_pos == '\"')
						{
							str_cmd_inf->is_in_doublequote = 0;
							break ;
						}
					}
					else
					{
						if (*cur_pos == '\'')
							str_cmd_inf->is_in_quote = 1;
						else if (*cur_pos == '\"')
							str_cmd_inf->is_in_doublequote = 1;
						if (*cur_pos != '{' && *cur_pos != '}')
						{
							err_code = SCMDERR_NOERROR;
							break ;
						}
					}
				}
				++cur_pos;
			}
		}
	}
	return (cur_pos);
}
