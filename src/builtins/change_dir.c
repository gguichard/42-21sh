/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_dir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/23 15:13:49 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/28 17:45:49 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "vars.h"
#include "options.h"
#include "builtins.h"

static t_opts	*cd_get_opts(int argc, char **argv)
{
	t_opts	*opts;

	opts = parse_opts(argc, argv, "");
	if (opts == NULL)
	{
		ft_dprintf(2, "%s: %s: unexpected error\n", ERR_PREFIX, argv[0]);
		return (NULL);
	}
	else if (opts->error != 0)
	{
		ft_dprintf(2, "%s: %s: -%c: invalid option\n", ERR_PREFIX, argv[0]
				, opts->error);
		ft_dprintf(2, "%s: usage: cd [dir]\n", argv[0]);
		ft_memdel((void *)&opts);
	}
	return (opts);
}

static char		*get_full_cur_path(t_shell *shell, const char *cur_path)
{
	t_var	*pwd;
	size_t	pwd_len;
	size_t	cur_len;
	char	*new_path;

	if ((cur_path)[0] == '/')
		new_path = ft_strdup(cur_path);
	else
	{
		pwd = get_var(shell->env, "PWD");
		pwd_len = (pwd == NULL) ? 0 : ft_strlen(pwd->value);
		if (pwd_len > 0 && (pwd->value)[pwd_len - 1] == '/')
			pwd_len -= 1;
		cur_len = ft_strlen(cur_path);
		new_path = (char *)malloc((pwd_len + cur_len + 2) * sizeof(char));
		if (new_path == NULL)
			return (NULL);
		if (pwd != NULL)
			ft_memcpy(new_path, pwd->value, pwd_len);
		new_path[pwd_len] = '/';
		ft_memcpy(new_path + pwd_len + 1, cur_path, cur_len + 1);
	}
	if (new_path != NULL)
		set_dir_to_canonical_form(new_path);
	return (new_path);
}

static char		*get_cur_path(t_shell *shell, t_opts *opts, int argc
		, char **argv)
{
	char	*cur_path;
	t_var	*old_pwd;
	char	*tmp;

	cur_path = NULL;
	if (opts->index >= argc)
		cur_path = get_shell_var(shell, "HOME");
	else if (!ft_strequ(argv[opts->index], "-"))
		cur_path = ft_strdup(argv[opts->index]);
	else
	{
		old_pwd = get_var(shell->env, "OLDPWD");
		cur_path = old_pwd == NULL ? NULL : ft_strdup(old_pwd->value);
	}
	if (cur_path != NULL)
	{
		tmp = get_full_cur_path(shell, cur_path);
		free(cur_path);
		cur_path = tmp;
	}
	return (cur_path);
}

static int		change_dir(t_shell *shell, const char *cur_path)
{
	t_var	*pwd;

	if (chdir(cur_path) == 0)
	{
		pwd = get_var(shell->env, "PWD");
		update_var(&(shell->env), "OLDPWD", pwd == NULL ? "" : pwd->value);
		update_var(&(shell->env), "PWD", cur_path);
		return (0);
	}
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
		ft_dprintf(2, "%s: %s: unexpected error\n", ERR_PREFIX, argv[0]);
	}
	else
	{
		ret = change_dir(shell, cur_path);
		if (ret == 0 && opts->index < argc && ft_strequ(argv[opts->index], "-"))
			ft_printf("%s\n", cur_path);
		free(cur_path);
	}
	free(opts);
	return (ret);
}
