/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 14:34:46 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/22 16:53:41 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "execute.h"
#include "libft.h"

static void		del_pipe(void *content, size_t content_size)
{
	t_pipe	*pipe;

	(void)content_size;
	pipe = (t_pipe *)content;
	pipe->left_pipefd = NULL;
	if (!(pipe->is_rightmost))
	{
		close(pipe->right_pipefd[0]);
		close(pipe->right_pipefd[1]);
	}
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

static void		exec_pipe(t_shell *shell, t_pipe *pipe, const char *path)
{
	(void)shell;
	(void)pipe;
	(void)path;
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
	curr = pipeline;
	while (curr != NULL)
	{
		pipe = (t_pipe *)curr->content;
		exec_pipe(shell, pipe, path);
		curr = curr->next;
	}
	ft_lstdel(&pipeline, del_pipe);
}
