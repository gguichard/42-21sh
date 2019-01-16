/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_hooks2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 14:19:25 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/15 18:26:01 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"
#include "utils.h"

int			move_cursor_prev_word(t_shell *shell, t_term *term)
{
	if (term->cursor == 0)
		return (0);
	while (term->line[term->cursor - 1] == ' '
			&& move_cursor_left(shell, term))
		;
	while (term->line[term->cursor - 1] != ' '
			&& move_cursor_left(shell, term))
		;
	return (1);
}

int			move_cursor_next_word(t_shell *shell, t_term *term)
{
	if (term->cursor == term->size)
		return (0);
	while (term->line[term->cursor] != ' '
			&& move_cursor_right(shell, term))
		;
	while (term->line[term->cursor] == ' '
			&& move_cursor_right(shell, term))
		;
	return (1);
}

static void	fix_outbound_col(t_term *term)
{
	size_t	col;
	size_t	max_col;

	col = term->col;
	max_col = get_max_col(term);
	if (term->row == 0 && term->col < term->offset)
		col = term->offset;
	else if (term->col > max_col)
		col = max_col;
	if (col != term->col)
	{
		term->col = col;
		tputs(tparm(tgetstr("ch", NULL), term->col), 1, t_putchar);
	}
}

int			move_cursor_up(t_shell *shell, t_term *term)
{
	(void)shell;
	if (term->row == 0)
		return (0);
	(term->row)--;
	tputs(tgetstr("up", NULL), 1, t_putchar);
	fix_outbound_col(term);
	return (1);
}

int			move_cursor_down(t_shell *shell, t_term *term)
{
	(void)shell;
	if (term->row + 1 == term->rows)
		return (0);
	(term->row)++;
	tputs(tgetstr("do", NULL), 1, t_putchar);
	fix_outbound_col(term);
	return (1);
}
