/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 12:18:37 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/05 18:56:18 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "utils.h"

void	move_cursor_right_col(t_term *term)
{
	tputs(tparm(tgetstr("RI", NULL), term->winsize.ws_col), 1, term_putchar);
}

void	move_cursor_left(t_term *term)
{
	if (term->cursor <= 0)
	{
		tputs(tgetstr("bl", NULL), 1, term_putchar);
		return ;
	}
	if ((term->cursor + prompt_len(NULL)) % term->winsize.ws_col != 0)
		tputs(tgetstr("le", NULL), 1, term_putchar);
	else
	{
		move_cursor_right_col(term);
		tputs(tgetstr("up", NULL), 1, term_putchar);
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
	if ((term->cursor + prompt_len(NULL)) % term->winsize.ws_col != 0)
		tputs(tgetstr("nd", NULL), 1, term_putchar);
	else
	{
		tputs(tgetstr("cr", NULL), 1, term_putchar);
		tputs(tgetstr("do", NULL), 1, term_putchar);
	}
}

void	move_cursor_home(t_term *term)
{
	int	rows;

	if (term->cursor <= 0)
		return ;
	rows = (term->cursor + prompt_len(NULL)) / term->winsize.ws_col;
	if (rows > 0)
	{
		tputs(tparm(tgetstr("UP", NULL), rows), 1, term_putchar);
		term->cursor -= rows * term->winsize.ws_col;
	}
	tputs(tgetstr("cr", NULL), 1, term_putchar);
	
	if (term->cursor > 0)
	{
		tputs(tparm(tgetstr("LE", NULL), term->cursor), 1, term_putchar);
		term->cursor = 0;
	}
}

void	move_cursor_end(t_term *term)
{
	int	rows;
	int	offset;

	if (term->cursor >= term->size)
		return ;
	rows = term->size / term->winsize.ws_col;
	rows -= term->cursor / term->winsize.ws_col;
	if (rows > 0)
	{
		tputs(tparm(tgetstr("DO", NULL), rows), 1, term_putchar);
		term->cursor += term->winsize.ws_col;
	}
	offset = term->size - term->cursor;
	if (offset > 0)
	{
		tputs(tparm(tgetstr("RI", NULL), offset), 1, term_putchar);
		term->cursor = term->size;
	}
}
