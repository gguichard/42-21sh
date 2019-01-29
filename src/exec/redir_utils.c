/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 10:46:34 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/29 10:47:24 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "redirect_inf.h"

int	dup2_with_rc(t_redirect_inf *redirect_inf, int from_fd, int to_fd)
{
	t_redirect_fd	redirect_fd;
	t_list			*elem;

	redirect_fd.original_fd = dup(to_fd);
	redirect_fd.from_fd = from_fd;
	redirect_fd.to_fd = to_fd;
	elem = ft_lstnew(&redirect_fd, sizeof(t_redirect_fd));
	if (elem == NULL || dup2(from_fd, to_fd) < 0)
		return (0);
	ft_lstadd(&(redirect_inf->red_fd), elem);
	return (1);
}
