/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 08:11:17 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/03 16:43:33 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "libft.h"
#include "error.h"

t_error		check_dir_for_cd(const char *path)
{
	struct stat		stat_buf;
	char			*cur_path;
	char			*cur_slash;
	char			old_char;

	if ((cur_path = ft_strdup(path)) == NULL)
		return (ERRC_UNEXPECTED);
	cur_slash = cur_path;
	while (cur_slash != NULL)
	{
		if ((cur_slash = ft_strchr(cur_slash, '/')) != NULL)
		{
			old_char = cur_slash[cur_path == cur_slash ? 1 : 0];
			cur_slash[cur_path == cur_slash ? 1 : 0] = '\0';
		}
		if (stat(cur_path, &stat_buf) == -1)
			return (del_then_ret_err(cur_path, ERRC_FILENOTFOUND));
		else if (!S_ISDIR(stat_buf.st_mode))
			return (del_then_ret_err(cur_path, ERRC_NOTADIR));
		else if (access(cur_path, X_OK) == -1)
			return (del_then_ret_err(cur_path, ERRC_NOEXECRIGHT));
		if (cur_slash != NULL && ++cur_slash != NULL)
			*(cur_slash - (cur_path == (cur_slash - 1) ? 0 : 1)) = old_char;
	}
	return (del_then_ret_err(cur_path, ERRC_NOERROR));
}

t_error		check_dir_of_file_for_cd(const char *path)
{
	char			*path_cpy;
	size_t			path_size;
	char			*last_slash;
	t_error			check_cd_ret;

	if ((path_cpy = ft_strdup(path)) == NULL)
		return (ERRC_UNEXPECTED);
	path_size = ft_strlen(path_cpy);
	if (path_size > 0 && path_cpy[path_size - 1] == '/')
	{
		while (path_size > 1 && path_cpy[path_size - 2] == '/')
		{
			path_cpy[path_size - 1] = '\0';
			--path_size;
		}
		path_cpy[path_size - 1] = '\0';
	}
	check_cd_ret = ERRC_NOERROR;
	if ((last_slash = ft_strrchr(path_cpy, '/')) != NULL)
	{
		last_slash[1] = '\0';
		check_cd_ret = check_dir_for_cd(path_cpy);
	}
	return (del_then_ret_err(path_cpy, check_cd_ret));
}

t_error		check_file_for_exec(const char *path)
{
	char			*path_cpy;
	size_t			path_size;
	int				end_with_slash;
	t_error			check_cd_ret;
	struct stat		stat_buf;

	if ((path_cpy = ft_strdup(path)) == NULL)
		return (ERRC_UNEXPECTED);
	if ((path_size = ft_strlen(path_cpy)) < 1)
		return (del_then_ret_err(path_cpy, ERRC_NOERROR));
	if ((end_with_slash = (path_cpy[path_size - 1] == '/')))
		path_cpy[path_size - 1] = '\0';
	if ((check_cd_ret = check_dir_of_file_for_cd(path_cpy)) != ERRC_NOERROR)
		return (del_then_ret_err(path_cpy, check_cd_ret));
	if (stat(path_cpy, &stat_buf) == -1)
		return (del_then_ret_err(path_cpy, ERRC_FILENOTFOUND));
	else if (end_with_slash && !S_ISDIR(stat_buf.st_mode))
		return (del_then_ret_err(path_cpy, ERRC_NOTADIR));
	else if (S_ISDIR(stat_buf.st_mode))
		return (del_then_ret_err(path_cpy, ERRC_ISADIR));
	else if (access(path_cpy, X_OK) == -1)
		return (del_then_ret_err(path_cpy, ERRC_NOEXECRIGHT));
	return (del_then_ret_err(path_cpy, ERRC_NOERROR));
}
