/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 09:53:07 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/22 17:58:36 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "cmd_inf.h"
#include "join_token_cmd.h"
#include "vars.h"
#include "execute.h"
#include "input.h"
#include "check_path.h"

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

static void	simple_fork(t_shell *shell, t_cmd_inf *cmd_inf
		, const char *bin_path, char **args)
{
	shell->last_fork_pid = fork();
	if (shell->last_fork_pid < 0)
		ft_dprintf(2, "%s: %s: Unable to fork\n", ERR_PREFIX, bin_path);
	else if (shell->last_fork_pid == 0)
		child_exec_cmd_inf(shell, cmd_inf, bin_path, args);
	else
	{
		waitpid(shell->last_fork_pid, &(shell->last_status), 0);
		shell->last_fork_pid = 0;
	}
}

static void	execute_cmd_inf(t_shell *shell, t_cmd_inf *cmd_inf, char *path)
{
	t_error	error;
	char	*bin_path;
	char	**args;

	error = ERRC_UNEXPECTED;
	bin_path = get_cmd_inf_path(cmd_inf, path, &error);
	if (error != ERRC_NOERROR)
	{
		ft_dprintf(2, "%s: %s: %s\n", ERR_PREFIX
				, cmd_inf->arg_lst->content, error_to_str(error));
		return ;
	}
	args = arg_lst_to_tab(cmd_inf->arg_lst);
	if (args != NULL)
	{
		reset_term(shell);
		simple_fork(shell, cmd_inf, bin_path, args);
		setup_term(shell);
	}
	free(bin_path);
	free(args);
}

void		execute_all(t_shell *shell, t_list *all_sub_cmd)
{
	t_var		*path;
	t_list		*cmd_lst;
	t_list		*cur_cmd;
	t_cmd_inf	*cmd_inf;

	path = get_var(shell->env, "PATH");
	if (path == NULL)
		return ;
	cmd_lst = join_token_cmd(all_sub_cmd);
	cur_cmd = cmd_lst;
	while (cur_cmd != NULL)
	{
		cmd_inf = (t_cmd_inf *)cur_cmd->content;
		if (cmd_inf->pipe_cmd == NULL)
			execute_cmd_inf(shell, cmd_inf, path->value);
		else
			execute_pipeline(shell, cmd_inf, path->value);
		cur_cmd = cur_cmd->next;
	}
	ft_lstdel(&cmd_lst, del_cmd);
}
