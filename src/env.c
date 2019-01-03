/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 14:39:11 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/03 17:13:42 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

t_list	*parse_env(char **environ)
{
	int		index;
	t_list	*lst;
	t_var	var;
	char	*sep;

	index = 0;
	lst = NULL;
	while (environ[index] != NULL)
	{
		sep = ft_strchr(environ[index], '=');
		if (sep == NULL)
			return (ft_lstfree(&lst));
		var.key = ft_strndup(environ[index], (size_t)(sep - environ[index]));
		var.value = ft_strdup(sep + 1);
		if (var.key == NULL || var.value == NULL)
			return (ft_lstfree(&lst));
		ft_lstpush(&lst, ft_lstnew(&var, sizeof(t_var)));
		index++;
	}
	return (lst);
}
