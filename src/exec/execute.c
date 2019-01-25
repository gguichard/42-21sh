/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 09:53:07 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/25 16:59:14 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "cmd_inf.h"
#include "join_token_cmd.h"
#include "check_path.h"
#include "vars.h"
#include "execute.h"

char		*get_cmd_inf_path(t_cmd_inf *cmd_inf, const char *path
		, t_error *error)
{
	char	*name;
	char	*bin_path;

	name = (char *)(cmd_inf->arg_lst->content);
	if (!ft_strchr(name, '/'))
		bin_path = search_binary(path, name, error);
	else
	{
		if ((bin_path = ft_strdup(name)) == NULL)
		{
			*error = ERRC_UNEXPECTED;
			return (NULL);
		}
		*error = check_file_rights(bin_path, FT_FILE, X_OK);
	}
	return (bin_path);
}

void		child_exec_cmd_inf(t_shell *shell, t_cmd_inf *cmd_inf
		, const char *bin_path, char **args)
{
	char	**environ;

	environ = NULL;
	if (fork_redirect(cmd_inf))
	{
		environ = get_environ_from_list(shell->env);
		execve(bin_path, args, environ);
	}
	free(environ);
	exit(0);
}

static void	execute_cmd_inf(t_shell *shell, t_cmd_inf *cmd_inf
		, const char *path)
{
	if (cmd_inf->pipe_cmd != NULL)
		execute_pipeline(shell, cmd_inf, path);
	else if (cmd_inf->arg_lst != NULL)
	{
		if (ft_strequ("cd", cmd_inf->arg_lst->content))
			builtin_cd(shell, ft_lstsize(cmd_inf->arg_lst)
					, arg_lst_to_tab(cmd_inf->arg_lst));
		else
			execute_single_cmd(shell, cmd_inf, path);
	}
}

void		execute_all(t_shell *shell, t_list *token_lst)
{
	t_list		*cmd_lst;
	t_list		*cur_cmd;
	t_var		*path;

	cmd_lst = join_token_cmd(token_lst, shell);
	cur_cmd = cmd_lst;
	while (cur_cmd != NULL)
	{
		process_redir(shell, (t_cmd_inf *)cur_cmd->content);
		cur_cmd = cur_cmd->next;
	}
	cur_cmd = cmd_lst;
	path = get_var(shell->env, "PATH");
	while (cur_cmd != NULL)
	{
		execute_cmd_inf(shell, (t_cmd_inf *)cur_cmd->content
				, path == NULL ? "" : path->value);
		cur_cmd = cur_cmd->next;
	}
	ft_lstdel(&cmd_lst, del_cmd);
}
