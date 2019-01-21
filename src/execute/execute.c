/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 09:53:07 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/21 17:39:09 by gguichar         ###   ########.fr       */
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

void	fork_child(t_shell *shell, t_cmd_inf *cmd_inf, char *path, char **args)
{
	char	**environ;

	shell->last_fork_pid = fork();
	if (shell->last_fork_pid < 0)
		ft_dprintf(2, "%s: unable to fork\n", ERR_PREFIX);
	else if (shell->last_fork_pid > 0)
	{
		waitpid(shell->last_fork_pid, &(shell->last_status), 0);
		shell->last_fork_pid = 0;
	}
	else
	{
		environ = NULL;
		if (fork_redirect(cmd_inf))
		{
			environ = get_environ_from_list(shell->env);
			execve(path, args, environ);
		}
		free(environ);
		exit(0);
	}
}

void	locate_and_exec(t_shell *shell, t_cmd_inf *cmd_inf, char **args)
{
	t_var	*path;
	char	*locate;
	t_error	error;

	path = get_var(shell->env, "PATH");
	if (path == NULL)
		return ;
	if (!ft_strchr(args[0], '/'))
		locate = search_binary(path->value, args[0], &error);
	else
	{
		locate = args[0];
		error = check_file_for_right(locate, X_OK);
	}
	if (error != ERRC_NOERROR)
		ft_dprintf(2, "%s: %s: %s\n", ERR_PREFIX, args[0], error_to_str(error));
	else if (error == ERRC_NOERROR)
	{
		reset_term(shell);
		fork_child(shell, cmd_inf, locate, args);
		setup_term(shell);
	}
}

void	execute_cmd_inf(t_shell *shell, t_cmd_inf *cmd_inf)
{
	char	**args;

	args = arg_lst_to_tab(cmd_inf->arg_lst);
	locate_and_exec(shell, cmd_inf, args);
	if (cmd_inf->pipe_cmd != NULL)
		execute_cmd_inf(shell, cmd_inf->pipe_cmd);
}

void	execute_all(t_shell *shell, t_list *all_sub_cmd)
{
	t_list		*cmd_lst;
	t_list		*cur_cmd;
	t_cmd_inf	*cmd_inf;

	cmd_lst = join_token_cmd(all_sub_cmd);
	cur_cmd = cmd_lst;
	while (cur_cmd != NULL)
	{
		cmd_inf = (t_cmd_inf *)cur_cmd->content;
		execute_cmd_inf(shell, cmd_inf);
		cur_cmd = cur_cmd->next;
	}
}
