/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 14:34:46 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/23 00:47:39 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "execute.h"
#include "input.h"
#include "libft.h"

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

static void		pipe_fork(t_shell *shell, t_pipe *pipe, const char *bin_path
		, char **args)
{
	pipe->fork_pid = fork();
	if (pipe->fork_pid < 0)
		ft_dprintf(2, "%s: %s: Unable to fork pipeline\n", ERR_PREFIX
				, bin_path);
	else if (pipe->fork_pid == 0)
	{
		if (!(pipe->is_leftmost))
		{
			dup2(pipe->in_fd, STDIN_FILENO);
			close(pipe->in_fd);
			close(pipe->fildes[1]);
		}
		if (!(pipe->is_rightmost))
		{
			dup2(pipe->out_fd, STDOUT_FILENO);
			close(pipe->out_fd);
			// TODO: close pipe write
		}
		child_exec_cmd_inf(shell, pipe->cmd_inf, bin_path, args);
	}
}

static void		exec_pipe(t_shell *shell, t_pipe *pipe, const char *path)
{
	t_error		error;
	char		*bin_path;
	char		**args;

	error = ERRC_UNEXPECTED;
	bin_path = get_cmd_inf_path(pipe->cmd_inf, path, &error);
	args = NULL;
	if (error == ERRC_NOERROR
			&& (args = arg_lst_to_tab(pipe->cmd_inf->arg_lst)) == NULL)
		error = ERRC_UNEXPECTED;
	if (error != ERRC_NOERROR)
	{
		free(bin_path);
		ft_dprintf(2, "%s: %s: %s\n", ERR_PREFIX
				, pipe->cmd_inf->arg_lst->content, error_to_str(error));
		return ;
	}
	pipe_fork(shell, pipe, bin_path, args);
	free(args);
}

void			execute_pipeline(t_shell *shell, t_cmd_inf *cmd_inf
		, const char *path)
{
	t_list	*pipeline;
	t_list	*curr;
	t_pipe	*pipe_elem;
	t_pipe	*next_pipe;

	pipeline = create_pipeline(cmd_inf);
	if (pipeline == NULL)
	{
		ft_dprintf(2, "%s: Unable to init pipeline\n", ERR_PREFIX);
		return ;
	}
	reset_term(shell);
	curr = pipeline;
	while (curr != NULL)
	{
		pipe_elem = (t_pipe *)curr->content;
		if (curr->next != NULL)
		{
			next_pipe = (t_pipe *)curr->next->content;
			if (pipe(next_pipe->fildes) == 0)
			{
				pipe_elem->out_fd = (next_pipe->fildes)[1];
				next_pipe->in_fd = (next_pipe->fildes)[0];
			}
		}
		exec_pipe(shell, pipe_elem, path);
		if (!(pipe_elem->is_leftmost))
		{
			close((pipe_elem->fildes)[0]);
			close((pipe_elem->fildes)[1]);
		}
		if (pipe_elem->fork_pid > 0)
			ft_lstpush(&(shell->fork_pids)
					, ft_lstnew(&(pipe_elem->fork_pid), sizeof(pid_t)));
		curr = curr->next;
	}
	curr = shell->fork_pids;
	while (curr != NULL)
	{
		waitpid(*((pid_t *)curr->content), NULL, 0);
		free(curr->content);
		curr = curr->next;
	}
	shell->fork_pids = NULL;
	ft_lstfree(&pipeline);
	setup_term(shell);
}
