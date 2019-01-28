/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 14:34:46 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/28 14:35:14 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "execute.h"

static t_list	*create_pipeline(t_cmd_inf *cmd_inf)
{
	t_cmd_inf	*cur_cmd_inf;
	t_list		*pipeline;
	t_list		*elem;
	t_pipe		data;

	cur_cmd_inf = cmd_inf;
	pipeline = NULL;
	while (cur_cmd_inf != NULL)
	{
		data.is_leftmost = cmd_inf == cur_cmd_inf;
		data.is_rightmost = cur_cmd_inf->pipe_cmd == NULL;
		data.in_fd = -1;
		data.out_fd = -1;
		data.cmd_inf = cur_cmd_inf;
		elem = ft_lstnew(&data, sizeof(t_pipe));
		if (elem == NULL)
			return (ft_lstfree(&pipeline));
		ft_lstpush(&pipeline, elem);
		cur_cmd_inf = cur_cmd_inf->pipe_cmd;
	}
	return (pipeline);
}

static pid_t	pipe_fork(t_shell *shell, t_list *curr, const char *bin_path
		, char **args)
{
	t_pipe	*pipe;
	pid_t	pid;

	pipe = (t_pipe *)curr->content;
	pid = fork();
	if (pid < 0)
		ft_dprintf(2, "%s: %s: Unable to fork\n", ERR_PREFIX
				, pipe->cmd_inf->arg_lst->content);
	else if (pid > 0)
		ft_lstpush(&(shell->fork_pids), ft_lstnew(&(pid), sizeof(pid_t)));
	else
	{
		if (!(pipe->is_leftmost) && pipe->in_fd != -1)
		{
			dup2(pipe->in_fd, STDIN_FILENO);
			close(pipe->in_fd);
			close((pipe->fildes)[1]);
		}
		if (!(pipe->is_rightmost) && pipe->out_fd != -1)
		{
			dup2(pipe->out_fd, STDOUT_FILENO);
			close(pipe->out_fd);
			close((((t_pipe *)curr->next->content)->fildes)[0]);
		}
		if (bin_path == NULL || args == NULL)
			exit(127);
		child_exec_cmd_inf(shell, pipe->cmd_inf, bin_path, args);
	}
	return (pid);
}

static int		exec_pipe(t_shell *shell, t_list *curr)
{
	t_error		error;
	t_pipe		*pipe;
	char		*bin_path;
	char		**args;

	error = ERRC_NOERROR;
	pipe = (t_pipe *)curr->content;
	bin_path = get_cmd_inf_path(shell, pipe->cmd_inf, &error);
	args = NULL;
	if (error == ERRC_NOERROR
			&& (args = arg_lst_to_tab(pipe->cmd_inf->arg_lst)) == NULL)
		error = ERRC_UNEXPECTED;
	if (error != ERRC_NOERROR)
		ft_dprintf(2, "%s: %s: %s\n", ERR_PREFIX
				, pipe->cmd_inf->arg_lst->content, error_to_str(error));
	pipe_fork(shell, curr, bin_path, args);
	free(bin_path);
	free(args);
	return (1);
}

static void		setup_pipes(t_shell *shell, t_list *pipeline)
{
	t_list	*curr;
	t_pipe	*curr_pipe;
	t_pipe	*next_pipe;

	curr = pipeline;
	while (curr != NULL)
	{
		curr_pipe = (t_pipe *)curr->content;
		if (curr->next != NULL)
		{
			next_pipe = (t_pipe *)curr->next->content;
			if (pipe(next_pipe->fildes) == 0)
			{
				curr_pipe->out_fd = (next_pipe->fildes)[1];
				next_pipe->in_fd = (next_pipe->fildes)[0];
			}
		}
		if (curr_pipe->cmd_inf->arg_lst != NULL && !exec_pipe(shell, curr))
		{
			kill_forked_pids(shell);
			break ;
		}
		if (!(curr_pipe->is_leftmost))
		{
			close((curr_pipe->fildes)[0]);
			close((curr_pipe->fildes)[1]);
		}
		curr = curr->next;
	}
}

void			execute_pipeline(t_shell *shell, t_cmd_inf *cmd_inf)
{
	t_list	*pipeline;
	t_list	*curr;
	t_list	*next;
	int		status;

	pipeline = create_pipeline(cmd_inf);
	if (pipeline == NULL)
	{
		ft_dprintf(2, "%s: Unable to init pipeline\n", ERR_PREFIX);
		return ;
	}
	setup_pipes(shell, pipeline);
	ft_lstfree(&pipeline);
	curr = shell->fork_pids;
	if (curr == NULL)
		shell->last_status = 127;
	else
	{
		while (curr != NULL)
		{
			waitpid(*((pid_t *)curr->content), &status, 0);
			next = curr->next;
			free(curr->content);
			free(curr);
			curr = next;
		}
		shell->last_status = WEXITSTATUS(status);
	}
	shell->fork_pids = NULL;
}
