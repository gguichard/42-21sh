/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visual_mode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 14:17:27 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/08 14:29:02 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"
#include "utils.h"

int	handle_vm_key(t_term *term, unsigned char key)
{
	(void)term;
	if (key != 'c' && key != 'x' && key != 'v')
		tputs(tgetstr("bl", NULL), 1, t_putchar);
	else if (key == 'c')
	{
	}
	else if (key == 'x')
	{
	}
	else if (key == 'v')
	{
	}
	return (1);
}
