/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 09:53:07 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/21 11:28:00 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "cmd_inf.h"
#include "join_token_cmd.h"
#include "vars.h"
#include "execute.h"
#include "input.h"
#include "check_path.h"

char	**arg_lst_to_tab(t_list *arg_lst)
{
	size_t	size;
	t_list	*curr;
	char	**args;
	size_t	index;

	size = 0;
	curr = arg_lst;
	while (curr != NULL)
	{
		size++;
		curr = curr->next;
	}
	if ((args = (char **)malloc((size + 1) * sizeof(char *))) != NULL)
	{
		curr = arg_lst;
		index = -1;
		while (++index < size && curr != NULL)
		{
			if ((args[index] = (char *)curr->content) == NULL)
				return (ft_strtab_free(args));
			curr = curr->next;
		}
		args[index] = NULL;
	}
	return (args);
}

void	fork_child(t_shell *shell, char *path, char **args)
{
	shell->last_fork_pid = fork();
	if (shell->last_fork_pid < 0)
		ft_dprintf(2, "%s: unable to fork\n", ERR_PREFIX);
	else if (shell->last_fork_pid > 0)
		waitpid(shell->last_fork_pid, NULL, 0);
	else
	{
		execve(path, args, NULL);
		exit(0);
	}
}

void	locate_and_exec(t_shell *shell, char **args)
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
		error = check_file_for_exec(locate);
	}
	if (error == ERRC_FILENOTFOUND)
		ft_dprintf(2, "%s: %s: Command not found\n", ERR_PREFIX, args[0]);
	else if (error == ERRC_NOEXECRIGHT)
		ft_dprintf(2, "%s: %s: Permission denied\n", ERR_PREFIX, args[0]);
	else if (error == ERRC_ISADIR)
		ft_dprintf(2, "%s: %s: Is a directory\n", ERR_PREFIX, args[0]);
	else if (error == ERRC_NOTADIR)
		ft_dprintf(2, "%s: %s: Not a directory\n", ERR_PREFIX, args[0]);
	else if (error == ERRC_NOERROR)
	{
		reset_term(shell);
		fork_child(shell, locate, args);
		setup_term(shell);
	}
}

void	execute_cmd(t_shell *shell, t_list *all_sub_cmd)
{
	t_list		*cmd_lst;
	t_list		*cur_cmd;
	t_cmd_inf	*cmd_inf;
	char		**args;

	cmd_lst = join_token_cmd(all_sub_cmd);
	cur_cmd = cmd_lst;
	while (cur_cmd != NULL)
	{
		cmd_inf = (t_cmd_inf *)cur_cmd->content;
		args = arg_lst_to_tab(cmd_inf->arg_lst);
		locate_and_exec(shell, args);
		cur_cmd = cur_cmd->next;
	}
}
