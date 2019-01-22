/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 12:06:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/22 12:06:52 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "execute.h"

int	redirect_input(t_redirect_inf *redirect_inf)
{
	int	fd;

	fd = open(redirect_inf->to_word, O_RDONLY);
	if (fd < 0)
	{
		ft_dprintf(2, "%s: %s: %s", ERR_PREFIX, redirect_inf->to_word,
				"Unable to open\n");
		return (0);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (1);
}
