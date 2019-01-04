/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 12:18:37 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/04 23:22:05 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "utils.h"

void	move_cursor_right_col(t_term *term)
{
	int	n;

	n = 0;
	while (n < term->winsize.ws_col)
	{
		tputs(tgetstr("nd", NULL), 1, term_putchar);
		n++;
	}
}

void	move_cursor_left(t_term *term)
{
	if (term->cursor <= 0)
	{
		tputs(tgetstr("bl", NULL), 1, term_putchar);
		return ;
	}
	if (term->cursor % term->winsize.ws_col != 0)
		tputs(tgetstr("le", NULL), 1, term_putchar);
	else
	{
		move_cursor_right_col(term);
		tputs(tgetstr("sr", NULL), 1, term_putchar);
	}
	(term->cursor)--;
}

void	move_cursor_right(t_term *term)
{
	if (term->cursor >= term->size)
	{
		tputs(tgetstr("bl", NULL), 1, term_putchar);
		return ;
	}
	(term->cursor)++;
	if (term->cursor % term->winsize.ws_col != 0)
		tputs(tgetstr("nd", NULL), 1, term_putchar);
	else
	{
		tputs(tgetstr("cr", NULL), 1, term_putchar);
		tputs(tgetstr("sf", NULL), 1, term_putchar);
	}
}
