/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_path_to_tab.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 14:46:36 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/28 13:51:32 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "vars.h"
#include "expand_vars.h"
#include "convert_path_to_tab.h"

/*
** Retourne le nombre de path dans la variable path.
*/

static size_t	count_subpath(const char *path)
{
	size_t	subpath_count;

	subpath_count = 1;
	while (*path != '\0')
	{
		if (*path == ':')
			++subpath_count;
		++path;
	}
	return (subpath_count);
}

char			**convert_path_to_tab(t_shell *shell)
{
	char	*path_cpy;
	size_t	path_count;
	char	**path_tab;
	char	*next_sep;
	size_t	path_idx;

	if ((path_cpy = get_shell_var(shell, "PATH")) == NULL)
		return (NULL);
	path_count = count_subpath(path_cpy);
	if ((path_tab = (char**)malloc(sizeof(char*) * (path_count + 1))) == NULL)
		return (NULL);
	path_tab[path_count] = NULL;
	path_idx = 0;
	while (path_idx < path_count)
	{
		next_sep = ft_strchr(path_cpy, ':');
		if (next_sep == path_cpy || *path_cpy == '\0')
			path_tab[path_idx] = ft_strdup(".");
		else
		{
			if (next_sep != NULL)
				*next_sep = '\0';
			path_tab[path_idx] = expand_home(path_cpy, shell, 0);
		}
		if (path_tab[path_idx] == NULL)
			return (ft_strtab_free(path_tab));
		if (next_sep != NULL)
			path_cpy = next_sep + 1;
		++path_idx;
	}
	return (path_tab);
}
