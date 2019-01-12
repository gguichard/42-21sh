/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd_token.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 14:15:31 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/12 16:15:00 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "str_cmd_inf.h"
#include "token_inf.h"
#include "split_cmd_token.h"

/*
** Ajout du token represente par les informations passes en parametre
** a la liste des commandes. Retourne 1 en cas de succes et 0 en cas d'erreur.
*/

static int	add_cur_token_to_lst(t_list **token_lst, t_str_cmd_inf *str_cmd_inf,
		const char *token_start, t_token_type token_type)
{
	t_token_inf		new_token;
	t_list			*new_elem;
	size_t			cur_token_size;

	cur_token_size = (str_cmd_inf->str + str_cmd_inf->pos)
		- token_start + (token_type == TK_OPE ? 1 : 0);
	new_token.token = ft_strndup(token_start, cur_token_size);
	new_token.type = token_type;
	if (new_token.token == NULL
			|| (new_elem = ft_lstnew(&new_token, sizeof(t_token_inf))) == NULL)
	{
		free(new_token.token);
		return (0);
	}
	ft_lstpush(token_lst, new_elem);
	return (1);
}

static int	is_a_sep_char(char c)
{
	return (c == ' ' || c == '\t' || c == '|'
			|| c == '<' || c == '>' || c == ';');
}

static int	cur_token_is_number(t_str_cmd_inf *str_cmd_inf,
		const char *token_start)
{
	while (token_start != str_cmd_inf->str + str_cmd_inf->pos)
	{
		if (!ft_isdigit(*token_start))
			return (0);
		++token_start;
	}
	return (1);
}

static int	get_cur_token_len(const char *token_start)
{
	size_t	len;

	len = 0;
	if (is_a_sep_char(*token_start))
	{
		++len;
		if ((*token_start == '>' || *token_start == '<')
				&& *token_start == token_start[1])
			++len;
	}
	return (len);
}

t_list		*split_cmd_token(t_str_cmd_inf *str_cmd_inf)
{
	int				last_char_was_sep;
	t_list			*token_lst;
	t_token_type	token_type;
	const char		*token_start;

	last_char_was_sep = 1;
	token_lst = NULL;
	token_type = TK_WORD;
	token_start = str_cmd_inf->str;
	while (str_cmd_inf->str[str_cmd_inf->pos] != '\0')
	{
		if (scmd_cur_char_is_in_nothing(str_cmd_inf)
				&& is_a_sep_char(str_cmd_inf->str[str_cmd_inf->pos])
				&& !scmd_cur_char_is_escaped(str_cmd_inf))
		{
			if (!last_char_was_sep)
			{
				if ((str_cmd_inf->str[str_cmd_inf->pos] == '>'
							|| str_cmd_inf->str[str_cmd_inf->pos] == '<')
						&& cur_token_is_number(str_cmd_inf, token_start))
					token_type = TK_NUM_OPT;
				if (!add_cur_token_to_lst(&token_lst, str_cmd_inf,
							token_start, token_type))
					return (ft_lstfree(&token_lst));
			}
			token_start = str_cmd_inf->str + str_cmd_inf->pos;
			if (str_cmd_inf->str[str_cmd_inf->pos] != ' '
					&& str_cmd_inf->str[str_cmd_inf->pos] != '\t')
			{
				token_type = TK_OPE;
				str_cmd_inf->pos += get_cur_token_len(token_start) - 1;
				if (!add_cur_token_to_lst(&token_lst, str_cmd_inf,
							token_start, token_type))
					return (ft_lstfree(&token_lst));
			}
			last_char_was_sep = 1;
		}
		else
		{
			token_type = TK_WORD;
			if (last_char_was_sep)
				token_start = str_cmd_inf->str + str_cmd_inf->pos;
			last_char_was_sep = 0;
		}
		scmd_move_to_next_char(str_cmd_inf);
	}
	if (token_type == TK_WORD && !add_cur_token_to_lst(&token_lst,
				str_cmd_inf, token_start, token_type))
		return (ft_lstfree(&token_lst));
	return (token_lst);
}
