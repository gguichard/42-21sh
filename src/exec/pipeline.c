/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 14:34:46 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/22 18:52:12 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "execute.h"
#include "input.h"
#include "libft.h"

static void		del_pipe(void *content, size_t content_size)
{
	t_pipe	*pipe;

	(void)content_size;
	pipe = (t_pipe *)content;
	if (pipe->left_pipefd != NULL)
	{
		close((pipe->left_pipefd)[0]);
		close((pipe->left_pipefd)[1]);
	}
	pipe->left_pipefd = NULL;
	free(content);
}

static t_list	*create_pipeline(t_cmd_inf *cmd_inf)
{
	t_list		*pipeline;
	t_list		*elem;
	t_cmd_inf	*cur_cmd_inf;
	t_pipe		data;
	t_pipe		*prev_pipe;

	pipeline = NULL;
	prev_pipe = NULL;
	cur_cmd_inf = cmd_inf;
	while (cur_cmd_inf != NULL)
	{
		data.is_leftmost = cmd_inf == cur_cmd_inf;
		data.is_rightmost = cur_cmd_inf->pipe_cmd == NULL;
		data.left_pipefd = (prev_pipe == NULL) ? NULL : prev_pipe->right_pipefd;
		if (!data.is_rightmost && pipe(data.right_pipefd) < 0)
			return (ft_lstdel(&pipeline, del_pipe));
		data.cmd_inf = cur_cmd_inf;
		elem = ft_lstnew(&data, sizeof(t_pipe));
		if (elem == NULL)
			return (ft_lstdel(&pipeline, del_pipe));
		ft_lstpush(&pipeline, elem);
		prev_pipe = (t_pipe *)elem->content;
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
	else if (pipe->fork_pid > 0)
	{
		if (pipe->left_pipefd != NULL)
		{
			close((pipe->left_pipefd)[0]);
			close((pipe->left_pipefd)[1]);
			pipe->left_pipefd = NULL;
		}
	}
	else
	{
		if (!(pipe->is_leftmost))
		{
			dup2((pipe->left_pipefd)[0], STDIN_FILENO);
			close((pipe->left_pipefd)[1]);
		}
		if (!(pipe->is_rightmost))
		{
			dup2((pipe->right_pipefd)[1], STDOUT_FILENO);
			close((pipe->right_pipefd)[0]);
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
	t_pipe	*pipe;

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
		pipe = (t_pipe *)curr->content;
		exec_pipe(shell, pipe, path);
		if (pipe->fork_pid > 0)
			ft_lstpush(&(shell->fork_pids)
					, ft_lstnew(&(pipe->fork_pid), sizeof(pid_t)));
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
	ft_lstdel(&pipeline, del_pipe);
	setup_term(shell);
}
