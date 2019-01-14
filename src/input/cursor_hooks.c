/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 12:18:37 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/14 13:34:44 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "utils.h"

int	get_max_col(t_term *term)
{
	/*int	cursor;

	cursor = term->cursor;
	while (cursor > 0 && (term->line)[cursor] != '\n')
		cursor--;
	return ((term->cursor - cursor) % term->winsize.ws_col);*/
	return (term->winsize.ws_col - 1);
}

int	move_cursor_left(t_shell *shell, t_term *term)
{
	(void)shell;
	if (term->row == 0 && term->col == term->offset)
		return (0);
	(term->cursor)--;
	if (term->col > term->offset
			|| (term->col > 0 && term->row > 0))
	{
		(term->col)--;
		tputs(tgetstr("le", NULL), 1, t_putchar);
	}
	else
	{
		(term->row)--;
		term->col = get_max_col(term);
		tputs(tgetstr("up", NULL), 1, t_putchar);
		tputs(tparm(tgetstr("ch", NULL), term->col), 1, t_putchar);
	}
	return (1);
}

int	move_cursor_right(t_shell *shell, t_term *term)
{
	(void)shell;
	if (term->cursor == term->size)
		return (0);
	(term->cursor)++;
	if ((term->col + 1) < term->winsize.ws_col
			&& (term->line)[term->cursor] != '\n')
	{
		(term->col)++;
		tputs(tgetstr("nd", NULL), 1, t_putchar);
	}
	else
	{
		(term->row)++;
		term->col = 0;
		tputs(tgetstr("cr", NULL), 1, t_putchar);
		tputs(tgetstr("do", NULL), 1, t_putchar);
	}
	return (1);
}

int	move_cursor_home(t_shell *shell, t_term *term)
{
	(void)shell;
	if (term->cursor == 0)
		return (0);
	term->cursor = 0;
	if (term->row > 0)
	{
		tputs(tparm(tgetstr("UP", NULL), term->row), 1, t_putchar);
		term->row = 0;
	}
	term->col = term->offset;
	tputs(tparm(tgetstr("ch", NULL), term->col), 1, t_putchar);
	return (1);
}

int	move_cursor_end(t_shell *shell, t_term *term)
{
	if (term->cursor == term->size)
		return (0);
	while (move_cursor_right(shell, term))
		;
	return (1);
}
