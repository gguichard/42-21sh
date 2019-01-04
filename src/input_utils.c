/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 22:06:12 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/04 23:23:23 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "shell.h"
#include "input.h"

int		setup_cmdline(t_term *term)
{
	term->capacity = CMDLINE_CAPACITY;
	if (!(term->line = (char *)malloc(term->capacity + 1)))
	{
		term->capacity = -1;
		return (0);
	}
	term->line[0] = '\0';
	term->size = 0;
	term->cursor = 0;
	return (1);
}

void	update_winsize(t_term *term)
{
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &(term->winsize));
}
