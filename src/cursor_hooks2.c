/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_hooks2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 14:19:25 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/05 15:07:56 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"
#include "utils.h"

void	move_cursor_prev_word(t_term *term)
{
	if (term->cursor <= 0)
	{
		tputs(tgetstr("bl", NULL), 1, term_putchar);
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
		tputs(tgetstr("bl", NULL), 1, term_putchar);
		return ;
	}
	while (term->cursor < term->size && term->line[term->cursor] != ' ')
		move_cursor_right(term);
	while (term->cursor < term->size && term->line[term->cursor] == ' ')
		move_cursor_right(term);
}
