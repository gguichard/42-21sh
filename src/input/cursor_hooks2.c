/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_hooks2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 14:19:25 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/14 15:29:03 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"
#include "utils.h"

int	move_cursor_prev_word(t_shell *shell, t_term *term)
{
	if (term->row == 0
			&& term->col == term->offset)
		return (0);
	while (term->line[term->cursor - 1] == ' '
			&& move_cursor_left(shell, term))
		;
	while (term->line[term->cursor - 1] != ' '
			&& move_cursor_left(shell, term))
		;
	return (1);
}

int	move_cursor_next_word(t_shell *shell, t_term *term)
{
	if (term->row + 1 == term->rows
			&& term->col == get_max_col(term))
		return (0);
	while (term->line[term->cursor] != ' '
			&& move_cursor_right(shell, term))
		;
	while (term->line[term->cursor] == ' '
			&& move_cursor_right(shell, term))
		;
	return (1);
}

int	move_cursor_up(t_shell *shell, t_term *term)
{
	(void)shell;
	if (term->row == 0)
		return (0);
	(term->row)--;
	tputs(tgetstr("up", NULL), 1, t_putchar);
	return (1);
}

int	move_cursor_down(t_shell *shell, t_term *term)
{
	(void)shell;
	if (term->row + 1 == term->rows)
		return (0);
	(term->row)++;
	tputs(tgetstr("do", NULL), 1, t_putchar);
	return (1);
}
