/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_mode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 14:28:03 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/08 14:28:28 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"

int	handle_key(t_term *term, char key)
{
	if (key == '\n')
	{
		ft_putchar('\n');
		return (0);
	}
	else if (key == 127)
	{
		handle_bs_key(term);
		return (1);
	}
	insert_cmdline(term, key);
	return (1);
}
