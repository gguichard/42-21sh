/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 14:39:11 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/03 18:25:28 by gguichar         ###   ########.fr       */
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
		if (!(def = ft_strdup(environ[index])))
			ret = 0;
		if (ret && (sep = ft_strchr(def, '=')) == NULL)
			ret = 0;
		*sep = '\0';
		if (ret && !create_var(&lst, def, sep + 1))
			ret = 0;
		free(def);
		if (!ret)
			return (ft_lstdel(&lst, free_var));
		index++;
	}
	return (lst);
}
