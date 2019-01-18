/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 12:40:55 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/18 13:28:28 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "shell.h"
#include "convert_path_to_tab.h"
#include "check_path.h"
#include "execute.h"

static char		*get_binary_path(const char *path, const char *binary)
{
	size_t	path_len;
	size_t	binary_len;
	char	*file_path;

	path_len = ft_strlen(path);
	binary_len = ft_strlen(binary);
	file_path = (char *)malloc(path_len + binary_len + 2);
	if (file_path == NULL)
		return (NULL);
	ft_memcpy(file_path, path, path_len);
	file_path[path_len] = '/';
	ft_memcpy(&(file_path[path_len + 1]), binary, binary_len + 1);
	return (file_path);
}

static t_error	check_is_valid_path(const char *file_path)
{
	t_error	error;

	error = check_file_for_exec(file_path);
	if (error == ERRC_NOEXECRIGHT)
	{
		if (check_dir_of_file_for_cd(file_path) == ERRC_NOEXECRIGHT)
			return (ERRC_FILENOTFOUND);
	}
	return (error);
}

char			*search_binary(const char *path, const char *binary
		, t_error *error)
{
	char	*path_cpy;
	char	**path_tab;
	char	*file_path;
	size_t	index;

	if ((path_cpy = ft_strdup(path)) == NULL)
		return (NULL);
	file_path = NULL;
	if ((path_tab = convert_path_to_tab(path_cpy)) != NULL)
	{
		index = -1;
		while (path_tab[++index] != NULL)
		{
			if ((file_path = get_binary_path(path_tab[index], binary)) == NULL
					&& (*error = ERRC_UNEXPECTED) == ERRC_UNEXPECTED)
				break ;
			*error = check_is_valid_path(file_path);
			if (*error == ERRC_NOERROR || *error == ERRC_NOEXECRIGHT)
				break ;
			ft_strdel(&file_path);
		}
	}
	free(path_tab);
	free(path_cpy);
	return (file_path);
}
