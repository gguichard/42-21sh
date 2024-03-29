/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 10:46:34 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/29 19:54:21 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "redirect_inf.h"

int	dup2_with_rc(t_redirect_inf *redirect_inf, int from_fd, int to_fd)
{
	t_redirect_save	redirect_save;
	t_list			*elem;

	redirect_save.original_fd = dup(to_fd);
	if (from_fd < 10 && redirect_save.original_fd == from_fd)
	{
		close(redirect_save.original_fd);
		return (0);
	}
	redirect_save.from_fd = from_fd;
	redirect_save.to_fd = to_fd;
	elem = ft_lstnew(&redirect_save, sizeof(t_redirect_save));
	if (elem == NULL || dup2(from_fd, to_fd) < 0)
		return (0);
	ft_lstadd(&(redirect_inf->red_save), elem);
	return (1);
}

int	close_with_rc(t_redirect_inf *redirect_inf, int fd)
{
	t_redirect_save	redirect_save;
	t_list			*elem;

	redirect_save.original_fd = dup(fd);
	redirect_save.from_fd = -1;
	redirect_save.to_fd = fd;
	elem = ft_lstnew(&redirect_save, sizeof(t_redirect_save));
	if (elem == NULL)
		return (0);
	close(fd);
	ft_lstadd(&(redirect_inf->red_save), elem);
	return (1);
}
