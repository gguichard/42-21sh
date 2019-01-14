/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_hooks2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 14:19:25 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/14 12:51:32 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"
#include "utils.h"

int	move_cursor_prev_word(t_shell *shell, t_term *term)
{
	if (term->cursor == 0)
		return (0);
	while (term->cursor > 0 && term->line[term->cursor - 1] == ' ')
		move_cursor_left(shell, term);
	while (term->cursor > 0 && term->line[term->cursor - 1] != ' ')
		move_cursor_left(shell, term);
	return (1);
}

int	move_cursor_next_word(t_shell *shell, t_term *term)
{
	if (term->cursor == term->size)
		return (0);
	while (term->cursor < term->size && term->line[term->cursor] != ' ')
		move_cursor_right(shell, term);
	while (term->cursor < term->size && term->line[term->cursor] == ' ')
		move_cursor_right(shell, term);
	return (1);
}

int	move_cursor_up(t_shell *shell, t_term *term)
{
	(void)shell;
	if (term->row == 0)
		return (0);
	(term->row)--;
	tputs(tgetstr("up", NULL), 1, t_putchar);
	/*term->cursor = ft_max(0, term->cursor - term->winsize.ws_col);
	col = (term->cursor + term->offset) % term->winsize.ws_col;
	if (col > 0)
		tputs(tparm(tgetstr("ch", NULL), col), 1, t_putchar);*/
	return (1);
}

int	move_cursor_down(t_shell *shell, t_term *term)
{
	(void)shell;
	if (term->row + 1 == term->rows)
		return (0);
	(term->row)++;
	tputs(tgetstr("do", NULL), 1, t_putchar);
	/*term->cursor = ft_min(term->size, term->cursor + term->winsize.ws_col);
	col = (term->cursor + term->offset) % term->winsize.ws_col;
	if (col > 0)
		tputs(tparm(tgetstr("ch", NULL), col), 1, t_putchar);*/
	return (1);
}
