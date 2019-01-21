/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 13:51:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/21 16:26:01 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "execute.h"
#include "redirect_inf.h"
#include "check_path.h"
#include "libft.h"

int			redirect_file(t_redirect_inf *redirect_inf, int append_to_file)
{
	t_error	error;
	int		fd;
	int		oflag;

	fd = redirect_inf->to_fd;
	if (fd == FD_ERROR)
		return (-1);
	if (fd == FD_DEFAULT)
		return (-1);
	if (fd == FD_NOTSET)
	{
		error = check_file_for_right(redirect_inf->to_word, W_OK);
		if (error == ERRC_FILENOTFOUND)
			error = check_dir_of_file_for_cd(redirect_inf->to_word);
		// TODO: check droit ecriture ^
		if (error != ERRC_NOERROR)
		{
			ft_dprintf(2, "%s: %s: %s\n", ERR_PREFIX, redirect_inf->to_word
					, error_to_str(error));
			return (-1);
		}
		oflag = O_CREAT | O_WRONLY;
		if (append_to_file)
			oflag |= O_APPEND;
		else
			oflag |= O_TRUNC;
		fd = open(redirect_inf->to_word, oflag, 0644);
		if (fd < 0)
		{
			ft_dprintf(2, "%s: %s: Unable to open\n", ERR_PREFIX
					, redirect_inf->to_word);
			return (-1);
		}
	}
	return (fd);
}

static void	redirect_close_fd(t_redirect_inf *redirect_inf)
{
	if (redirect_inf->from_fd >= 0)
		close(redirect_inf->from_fd);
	if (redirect_inf->from_fd == FD_AMPERSAND
			|| redirect_inf->from_fd == FD_DEFAULT)
		close(STDOUT_FILENO);
	if (redirect_inf->from_fd == FD_AMPERSAND)
		close(STDERR_FILENO);
}

static int	redirect_to_file(t_redirect_inf *redirect_inf)
{
	int	fd;

	fd = redirect_file(redirect_inf, redirect_inf->red_type == RD_RR);
	if (fd < 0)
		return (0);
	if (redirect_inf->from_fd >= 0)
		dup2(fd, redirect_inf->from_fd);
	if (redirect_inf->from_fd == FD_AMPERSAND
			|| redirect_inf->from_fd == FD_DEFAULT)
		dup2(fd, STDOUT_FILENO);
	if (redirect_inf->from_fd == FD_AMPERSAND)
		dup2(fd, STDERR_FILENO);
	close(fd);
	return (1);
}

int			fork_redirect(t_cmd_inf *cmd_inf)
{
	t_list			*curr;
	t_redirect_inf	*redirect_inf;

	curr = cmd_inf->redirect_lst;
	while (curr != NULL)
	{
		redirect_inf = (t_redirect_inf *)curr->content;
		if (redirect_inf->red_type == RD_R && redirect_inf->close_to_fd)
			redirect_close_fd(redirect_inf);
		else if (redirect_inf->red_type == RD_R
				|| redirect_inf->red_type == RD_RR)
		{
			if (!redirect_to_file(redirect_inf))
				return (0);
		}
		curr = curr->next;
	}
	return (1);
}
