/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 16:41:55 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/03 17:18:24 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static t_var	*get_var_lst(t_list *lst, const char *key)
{
	t_var	*var;

	while (lst != NULL)
	{
		var = (t_var *)lst->content;
		if (ft_strequ(key, var->key))
			return (var);
		lst = lst->next;
	}
	return (NULL);
}

const char		*get_var(t_list *lst, const char *key)
{
	t_var	*var;

	var = get_var_lst(lst, key);
	return (var == NULL ? "" : var->value);
}

char			*get_shell_var(t_shell *shell, const char *key)
{
	t_var	*var;
	int		tmp;

	var = NULL;
	if (ft_isalpha(key[0]) || key[0] == '_')
	{
		var = get_var_lst(shell->env, key);
		if (var == NULL)
			var = get_var_lst(shell->local, key);
	}
	else
	{
		if (ft_strequ(key, "?"))
			return (ft_itoa((unsigned char)shell->last_status));
		if (ft_isdigit(key[0]))
		{
			tmp = ft_atoi(key);
			return (ft_strdup(shell->argc <= tmp ? "" : shell->argv[tmp]));
		}
	}
	return (var == NULL ? "" : ft_strdup(var->value));
}
