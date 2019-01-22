/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 13:51:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/22 12:07:08 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "execute.h"



int			fork_redirect(t_cmd_inf *cmd_inf)
{
	t_list			*curr;
	t_redirect_inf	*redirect_inf;

	curr = cmd_inf->redirect_lst;
	while (curr != NULL)
	{
		redirect_inf = (t_redirect_inf *)curr->content;
		if (redirect_inf->red_type == RD_R || redirect_inf->red_type == RD_RR)
		{
			if (!redirect_output(redirect_inf))
				return (0);
		}
		else if (redirect_inf->red_type == RD_L)
		{
			if (!redirect_input(redirect_inf))
				return (0);
		}
		curr = curr->next;
	}
	return (1);
}
