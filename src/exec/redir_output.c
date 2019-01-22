/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 11:58:43 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/22 12:35:36 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "execute.h"
#include "check_path.h"

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

static int	redirect_file(t_redirect_inf *redirect_inf, int append_to_file)
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
		error = check_file_rights(redirect_inf->to_word, FT_FILE, W_OK);
		if (error == ERRC_FILENOTFOUND)
			error = check_dir_of_file_rights(redirect_inf->to_word
					, X_OK | W_OK);
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

static int	redirect_fd(t_redirect_inf *redirect_inf)
{
	int	fd;

	fd = redirect_inf->to_fd;
	if (fd == FD_AMPERSAND)
	{
		if (redirect_inf->from_fd != 1 && redirect_inf->from_fd != FD_DEFAULT)
			ft_dprintf(2, "%s: %s: ambiguous redirect\n", ERR_PREFIX
					, redirect_inf->to_word);
		else
		{
			redirect_inf->from_fd = FD_AMPERSAND;
			redirect_inf->to_fd = FD_NOTSET;
		}
	}
	if (fd < 0)
		fd = redirect_file(redirect_inf, redirect_inf->red_type == RD_RR);
	return (fd);
}

static int	redirect_to_fd(t_redirect_inf *redirect_inf)
{
	int	fd;
	int	dup_ret;

	fd = redirect_fd(redirect_inf);
	if (fd < 0)
		return (0);
	dup_ret = 1;
	if (redirect_inf->from_fd >= 0)
		dup_ret = dup2(fd, redirect_inf->from_fd);
	else
	{
		if (redirect_inf->from_fd == FD_AMPERSAND
				|| redirect_inf->from_fd == FD_DEFAULT)
			dup_ret = dup2(fd, STDOUT_FILENO);
		if (dup_ret > 0 && redirect_inf->from_fd == FD_AMPERSAND)
			dup_ret = dup2(fd, STDERR_FILENO);
	}
	if (dup_ret < 0)
	{
		ft_dprintf(2, "%s: %d: Bad file descriptor\n", ERR_PREFIX, fd);
		return (0);
	}
	if (redirect_inf->close_to_fd)
		close(fd);
	return (1);
}

int			redirect_output(t_redirect_inf *redirect_inf)
{
	if (redirect_inf->close_to_fd && redirect_inf->to_fd == FD_DEFAULT)
		redirect_close_fd(redirect_inf);
	else
	{
		if (!redirect_to_fd(redirect_inf))
			return (0);
	}
	return (1);
}
