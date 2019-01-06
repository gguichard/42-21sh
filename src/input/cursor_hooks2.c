/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_hooks2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 14:19:25 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/06 18:44:42 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"
#include "utils.h"

void	move_cursor_prev_word(t_term *term)
{
	if (term->cursor <= 0)
	{
		tputs(tgetstr("bl", NULL), 1, t_putchar);
		return ;
	}
	while (term->cursor > 0 && term->line[term->cursor - 1] == ' ')
		move_cursor_left(term);
	while (term->cursor > 0 && term->line[term->cursor - 1] != ' ')
		move_cursor_left(term);
}

void	move_cursor_next_word(t_term *term)
{
	if (term->cursor >= term->size)
	{
		tputs(tgetstr("bl", NULL), 1, t_putchar);
		return ;
	}
	while (term->cursor < term->size && term->line[term->cursor] != ' ')
		move_cursor_right(term);
	while (term->cursor < term->size && term->line[term->cursor] == ' ')
		move_cursor_right(term);
}

void	move_cursor_up(t_term *term)
{
	int	col;

	if (term->cursor + term->offset < term->winsize.ws_col)
	{
		tputs(tgetstr("bl", NULL), 1, t_putchar);
		return ;
	}
	tputs(tgetstr("cr", NULL), 1, t_putchar);
	tputs(tgetstr("up", NULL), 1, t_putchar);
	term->cursor = ft_max(0, term->cursor - term->winsize.ws_col);
	col = (term->cursor + term->offset) % term->winsize.ws_col;
	if (col > 0)
		tputs(tparm(tgetstr("ch", NULL), col), 1, t_putchar);
}

void	move_cursor_down(t_term *term)
{
	int	curr;
	int	rows;
	int	col;

	curr = (term->cursor + term->offset) / term->winsize.ws_col;
	rows = (term->size + term->offset) / term->winsize.ws_col;
	if (curr >= rows)
	{
		tputs(tgetstr("bl", NULL), 1, t_putchar);
		return ;
	}
	tputs(tgetstr("cr", NULL), 1, t_putchar);
	tputs(tgetstr("do", NULL), 1, t_putchar);
	term->cursor = ft_min(term->size, term->cursor + term->winsize.ws_col);
	col = (term->cursor + term->offset) % term->winsize.ws_col;
	if (col > 0)
		tputs(tparm(tgetstr("ch", NULL), col), 1, t_putchar);
}
