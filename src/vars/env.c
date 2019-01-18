/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 14:39:11 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/09 21:31:29 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "shell.h"
#include "vars.h"

t_list	*parse_env(char **environ)
{
	int		index;
	t_list	*lst;
	char	*def;
	char	*sep;
	int		ret;

	index = 0;
	lst = NULL;
	ret = 1;
	while (environ[index] != NULL)
	{
		if (!(def = ft_strdup(environ[index]))
				|| (sep = ft_strchr(def, '=')) == NULL)
			ret = 0;
		else
		{
			*sep = '\0';
			ret = create_var(&lst, def, sep + 1);
		}
		free(def);
		if (!ret)
			return (ft_lstdel(&lst, free_var));
		index++;
	}
	return (lst);
}
