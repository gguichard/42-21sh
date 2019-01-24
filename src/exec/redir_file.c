/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 15:14:35 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/24 16:01:36 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "execute.h"
#include "redirect_inf.h"
#include "check_path.h"

void	redirect_file(t_redirect_inf *redirect_inf)
{
	int		fd;
	t_error	error;

	fd = redirect_inf->to_fd;
	if (fd == FD_ERROR)
	{
		ft_dprintf(2, "%s: -1: %s\n", ERR_PREFIX, "Bad file descriptor");
		return ;
	}
	if (fd == FD_NOTSET)
	{
		fd = open(redirect_inf->to_word, O_CREAT | O_WRONLY, 0644);
		if (fd >= 0)
			close(fd);
		else
		{
			error = check_file_rights(redirect_inf->to_word, FT_FILE, W_OK);
			if (error == ERRC_FILENOTFOUND)
				error = check_dir_of_file_rights(redirect_inf->to_word
						, X_OK | W_OK);
			ft_dprintf(2, "%s: %s: %s\n", ERR_PREFIX, redirect_inf->to_word
					, error_to_str(error));
		}
	}
}

void	process_redir(t_list *redirect_lst)
{
	t_list	*cur_redir;

	cur_redir = redirect_lst;
	while (cur_redir != NULL)
	{
		redirect_file((t_redirect_inf *)cur_redir->content);
		cur_redir = cur_redir->next;
	}
}
