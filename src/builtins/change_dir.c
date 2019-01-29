/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_dir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/23 15:13:49 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/29 23:18:58 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "vars.h"
#include "options.h"
#include "error.h"
#include "check_path.h"
#include "builtins.h"

static t_opts	*cd_get_opts(int argc, char **argv)
{
	t_opts	*opts;

	opts = parse_opts(argc, argv, "");
	if (opts == NULL)
	{
		ft_dprintf(2, "%s: %s: Unexpected error\n", ERR_PREFIX, argv[0]);
		return (NULL);
	}
	else if (opts->error != 0)
	{
		ft_dprintf(2, "%s: %s: -%c: Invalid option\n", ERR_PREFIX, argv[0]
				, opts->error);
		ft_dprintf(2, "%s: usage: cd [dir]\n", argv[0]);
		ft_memdel((void *)&opts);
	}
	return (opts);
}

static char		*get_new_cur_path(t_shell *shell, const char *cur_path)
{
	char	*pwd;
	size_t	pwd_len;
	size_t	cur_len;
	char	*new_path;

	if ((cur_path)[0] == '/')
		new_path = ft_strdup(cur_path);
	else
	{
		pwd = get_shell_var(shell, "PWD");
		pwd_len = (pwd == NULL) ? 0 : ft_strlen(pwd);
		if (pwd_len > 0 && pwd[pwd_len - 1] == '/')
			pwd_len -= 1;
		cur_len = ft_strlen(cur_path);
		new_path = (char *)malloc((pwd_len + cur_len + 2) * sizeof(char));
		if (new_path != NULL)
		{
			if (pwd != NULL)
				ft_memcpy(new_path, pwd, pwd_len);
			new_path[pwd_len] = '/';
			ft_memcpy(new_path + pwd_len + 1, cur_path, cur_len + 1);
		}
		free(pwd);
	}
	return (new_path);
}

static char		*get_cur_path(t_shell *shell, t_opts *opts, int argc
		, char **argv)
{
	char	*cur_path;
	char	*tmp;

	cur_path = NULL;
	if (opts->index >= argc)
		cur_path = get_shell_var(shell, "HOME");
	else if (!ft_strequ(argv[opts->index], "-"))
		cur_path = ft_strdup(argv[opts->index]);
	else
		cur_path = get_shell_var(shell, "OLDPWD");
	if (cur_path != NULL)
	{
		tmp = get_new_cur_path(shell, cur_path);
		free(cur_path);
		cur_path = tmp;
		if (cur_path != NULL)
			set_dir_to_canonical_form(cur_path);
	}
	return (cur_path);
}

static int		change_dir(t_shell *shell, const char *cur_path, char **argv)
{
	t_var		*pwd;
	t_error		error;
	const char	*def_path;

	if (chdir(cur_path) == 0)
	{
		pwd = get_var(shell->env, "PWD");
		update_var(&(shell->env), "OLDPWD", pwd == NULL ? "" : pwd->value);
		update_var(&(shell->env), "PWD", cur_path);
		return (0);
	}
	error = check_dir_rights(cur_path, X_OK);
	def_path = argv[1];
	if (def_path == NULL)
		def_path = cur_path;
	ft_dprintf(2, "%s: %s: %s\n", argv[0], def_path, error_to_str(error));
	return (1);
}

int				builtin_cd(t_shell *shell, int argc, char **argv)
{
	t_opts	*opts;
	int		ret;
	char	*cur_path;

	if ((opts = cd_get_opts(argc, argv)) == NULL)
		return (1);
	ret = 0;
	cur_path = get_cur_path(shell, opts, argc, argv);
	if (cur_path == NULL)
	{
		ret = 1;
		ft_dprintf(2, "%s: %s: Unexpected error\n", ERR_PREFIX, argv[0]);
	}
	else
	{
		ret = change_dir(shell, cur_path, argv);
		if (ret == 0 && opts->index < argc && ft_strequ(argv[opts->index], "-"))
			ft_printf("%s\n", cur_path);
		free(cur_path);
	}
	free(opts);
	return (ret);
}
