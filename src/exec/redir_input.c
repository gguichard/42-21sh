/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 12:06:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/29 12:17:09 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "execute.h"

int	redirect_input(t_redirect_inf *redirect_inf)
{
	int	fd;
	int	should_close;

	fd = redirect_inf->to_fd;
	should_close = redirect_inf->close_to_fd;
	if (fd == FD_DEFAULT)
		fd = redirect_inf->from_fd;
	else if (fd == FD_NOTSET)
	{
		fd = open(redirect_inf->to_word, O_RDONLY);
		should_close = 1;
	}
	else if (fd == FD_AMPERSAND)
	{
		ft_dprintf(2, "%s: %s: Ambiguous redirect\n", ERR_PREFIX
				, redirect_inf->to_word);
		return (0);
	}
	if (fd < 0)
		return (0);
	dup2_with_rc(redirect_inf, fd, ft_max(STDIN_FILENO, redirect_inf->from_fd));
	if (should_close)
		close(fd);
	return (1);
}
