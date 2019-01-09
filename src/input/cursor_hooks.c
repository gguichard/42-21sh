/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 12:18:37 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/09 12:35:40 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "utils.h"

void	move_cursor_left(t_shell *shell, t_term *term)
{
	(void)shell;
	if (term->cursor <= 0)
	{
		tputs(tgetstr("bl", NULL), 1, t_putchar);
		return ;
	}
	if ((term->cursor + term->offset) % term->winsize.ws_col != 0)
		tputs(tgetstr("le", NULL), 1, t_putchar);
	else
	{
		tputs(tparm(tgetstr("ch", NULL), term->winsize.ws_col), 1, t_putchar);
		tputs(tgetstr("up", NULL), 1, t_putchar);
	}
	(term->cursor)--;
}

void	move_cursor_right(t_shell *shell, t_term *term)
{
	(void)shell;
	if (term->cursor >= term->size
			|| (term->visual_mode && term->cursor + 1 >= term->size))
	{
		tputs(tgetstr("bl", NULL), 1, t_putchar);
		return ;
	}
	(term->cursor)++;
	if ((term->cursor + term->offset) % term->winsize.ws_col != 0)
		tputs(tgetstr("nd", NULL), 1, t_putchar);
	else
	{
		tputs(tgetstr("cr", NULL), 1, t_putchar);
		tputs(tgetstr("do", NULL), 1, t_putchar);
	}
}

void	move_cursor_home(t_shell *shell, t_term *term)
{
	int	rows;

	(void)shell;
	if (term->cursor <= 0)
		return ;
	rows = (term->cursor + term->offset) / term->winsize.ws_col;
	if (rows > 0)
		tputs(tparm(tgetstr("UP", NULL), rows), 1, t_putchar);
	tputs(tparm(tgetstr("ch", NULL), term->offset), 1, t_putchar);
	term->cursor = 0;
}

void	move_cursor_end(t_shell *shell, t_term *term)
{
	int	col;
	int	curr;
	int	rows;

	(void)shell;
	if (term->cursor >= term->size)
		return ;
	col = (term->size + term->offset) % term->winsize.ws_col;
	tputs(tparm(tgetstr("ch", NULL), col), 1, t_putchar);
	curr = (term->cursor + term->offset) / term->winsize.ws_col;
	rows = (term->size + term->offset) / term->winsize.ws_col;
	if (curr < rows)
		tputs(tparm(tgetstr("DO", NULL), rows - curr), 1, t_putchar);
	term->cursor = term->size;
}
