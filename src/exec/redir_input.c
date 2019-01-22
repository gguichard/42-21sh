/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 12:06:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/22 17:58:54 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "execute.h"
#include "check_path.h"

static int	redirect_file_input(t_redirect_inf *redirect_inf)
{
	int		fd;
	t_error	error;

	fd = open(redirect_inf->to_word, O_RDONLY);
	if (fd < 0)
	{
		error = check_file_rights(redirect_inf->to_word, FT_FILE, R_OK);
		if (error != ERRC_NOERROR)
			ft_dprintf(2, "%s: %s: %s\n", ERR_PREFIX, redirect_inf->to_word
					, error_to_str(error));
		else
			ft_dprintf(2, "%s: %s: %s\n", ERR_PREFIX, redirect_inf->to_word
					, "Unable to open");
	}
	return (fd);
}

int			redirect_input(t_redirect_inf *redirect_inf)
{
	int	fd;
	int	should_close;

	fd = redirect_inf->to_fd;
	should_close = redirect_inf->close_to_fd;
	if (fd == FD_DEFAULT)
		fd = redirect_inf->from_fd;
	else if (fd == FD_NOTSET)
	{
		fd = redirect_file_input(redirect_inf);
		should_close = 1;
	}
	else if (fd == FD_AMPERSAND)
	{
		ft_dprintf(2, "%s: %s: ambiguous redirect\n", ERR_PREFIX
				, redirect_inf->to_word);
		return (0);
	}
	if (fd < 0)
		return (0);
	dup2(fd, redirect_inf->from_fd >= 0 ? redirect_inf->from_fd : STDIN_FILENO);
	if (should_close)
		close(fd);
	return (1);
}
