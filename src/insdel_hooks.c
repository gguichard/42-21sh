/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insdel_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 16:06:26 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/05 00:17:45 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"
#include "utils.h"

static void	delete_char(t_term *term)
{
	tputs(tgetstr("dc", NULL), 1, term_putchar);
	if (term->cursor < term->size)
	{
		tputs(tgetstr("sc", NULL), 1, term_putchar);
		ft_putstr(&(term->line[term->cursor]));
		if ((term->size - 1) % term->winsize.ws_col == 0)
		{
			tputs(tgetstr("cr", NULL), 1, term_putchar);
			tputs(tgetstr("sf", NULL), 1, term_putchar);
		}
		tputs(tgetstr("dc", NULL), 1, term_putchar);
		tputs(tgetstr("rc", NULL), 1, term_putchar);
	}
	(term->size)--;
}

void		handle_bs_key(t_term *term)
{
	if (term->cursor <= 0)
	{
		tputs(tgetstr("bl", NULL), 1, term_putchar);
		return ;
	}
	ft_memcpy(&(term->line[term->cursor - 1])
			, &(term->line[term->cursor])
			, term->size - term->cursor + 1);
	if (term->cursor % term->winsize.ws_col != 0)
		tputs(tgetstr("le", NULL), 1, term_putchar);
	else
	{
		move_cursor_right_col(term);
		tputs(tgetstr("sr", NULL), 1, term_putchar);
	}
	(term->cursor)--;
	delete_char(term);
}

void		handle_del_key(t_term *term)
{
	if (term->cursor >= term->size)
	{
		tputs(tgetstr("bl", NULL), 1, term_putchar);
		return ;
	}
	ft_memmove(&(term->line[term->cursor])
			, &(term->line[term->cursor + 1])
			, term->size - term->cursor + 1);
	delete_char(term);
}
