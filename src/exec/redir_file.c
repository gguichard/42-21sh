/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 15:14:35 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/24 17:33:00 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "execute.h"
#include "cmd_inf.h"
#include "redirect_inf.h"
#include "check_path.h"

static void	redirect_file(t_redirect_inf *redirect_inf)
{
	int		fd;
	int		is_write;
	t_error	error;

	fd = redirect_inf->to_fd;
	if (fd == FD_ERROR)
	{
		ft_dprintf(2, "%s: -1: Bad file descriptor\n", ERR_PREFIX);
		return ;
	}
	if (fd == FD_NOTSET)
	{
		is_write = (redirect_inf->red_type == RD_R
				|| redirect_inf->red_type == RD_RR);
		fd = open(redirect_inf->to_word
				, is_write ? (O_CREAT | O_WRONLY) : O_RDONLY, 0644);
		if (fd >= 0)
			close(fd);
		else
		{
			error = check_file_rights(redirect_inf->to_word, FT_FILE
					, is_write ? W_OK : R_OK);
			if (is_write && error == ERRC_FILENOTFOUND)
				error = check_dir_of_file_rights(redirect_inf->to_word
						, X_OK | (is_write ? W_OK : R_OK));
			ft_dprintf(2, "%s: %s: %s\n", ERR_PREFIX, redirect_inf->to_word
					, error_to_str(error));
		}
	}
}

void		process_redir(t_cmd_inf *cmd_inf)
{
	t_list	*cur_redir;

	cur_redir = cmd_inf->redirect_lst;
	while (cur_redir != NULL)
	{
		redirect_file((t_redirect_inf *)cur_redir->content);
		cur_redir = cur_redir->next;
	}
	if (cmd_inf->pipe_cmd != NULL)
		process_redir(cmd_inf->pipe_cmd);
}
