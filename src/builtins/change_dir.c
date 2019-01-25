/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_dir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/23 15:13:49 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/25 13:44:54 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "vars.h"
#include "options.h"

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

static char		*get_cd_dir_without_dir(t_shell *shell)
{
	t_var	*home;

	home = get_var(shell->env, "HOME");
	if (home == NULL || (home->value)[0] == '\0')
		return (NULL);
	return (ft_strdup(home->value));
}

static int		change_dir(t_shell *shell, char **cur_path)
{
	t_var	*pwd;
	size_t	pwd_len;
	size_t	cur_len;
	char	*new_path;

	pwd = get_var(shell->env, "PWD");
	if ((*cur_path)[0] != '/')
	{
		pwd_len = (pwd == NULL) ? 0 : ft_strlen(pwd->value);
		if (pwd_len > 0 && (pwd->value)[pwd_len - 1] == '/')
			pwd_len -= 1;
		cur_len = ft_strlen(*cur_path);
		if ((new_path = (char *)malloc((pwd_len + cur_len + 2) * sizeof(char)))
				== NULL)
			return (1);
		if (pwd != NULL)
			ft_memcpy(new_path, pwd->value, pwd_len);
		new_path[pwd_len] = '/';
		ft_memcpy(new_path + pwd_len + 1, *cur_path, cur_len + 1);
		free(*cur_path);
		*cur_path = new_path;
	}
	set_dir_to_canonical_form(*cur_path);
	if (chdir(*cur_path) == 0)
	{
		update_var(&(shell->env), "OLDPWD", pwd == NULL ? "" : pwd->value);
		update_var(&(shell->env), "PWD", *cur_path);
	}
	return (0);
}

int				builtin_cd(t_shell *shell, int argc, char **argv)
{
	t_opts	*opts;
	t_var	*old_pwd;
	char	*cur_path;
	int		ret;

	if ((opts = cd_get_opts(argc, argv)) == NULL)
		return (1);
	ret = 0;
	old_pwd = NULL;
	cur_path = NULL;
	if (opts->index >= argc)
		cur_path = get_cd_dir_without_dir(shell);
	else
	{
		if (!ft_strequ(argv[opts->index], "-"))
			cur_path = ft_strdup(argv[opts->index]);
		else if ((old_pwd = get_var(shell->env, "OLDPWD")) != NULL)
			cur_path = ft_strdup(old_pwd->value);
		else
			ft_dprintf(2, "%s: %s: OLDPWD not set\n", ERR_PREFIX, argv[0]);
	}
	if (cur_path == NULL)
		ret = 1;
	if (ret == 0)
	{
		ret = change_dir(shell, &cur_path);
		if (ret == 0 && opts->index < argc && ft_strequ(argv[opts->index], "-"))
			ft_printf("%s\n", cur_path);
	}
	free(opts);
	free(cur_path);
	return (ret);
}
