/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insdel_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 16:06:26 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/06 16:44:46 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"
#include "utils.h"

static void	delete_char(t_term *term)
{
	int	curr;
	int	rows;

	tputs(tgetstr("dc", NULL), 1, t_putchar);
	curr = (term->cursor + term->offset) / term->winsize.ws_col;
	rows = (term->size + term->offset) / term->winsize.ws_col;
	if (curr < rows)
	{
		tputs(tgetstr("sc", NULL), 1, t_putchar);
		ft_putstr(&(term->line[term->cursor]));
		if ((term->size + term->offset - 1) % term->winsize.ws_col == 0)
		{
			tputs(tgetstr("rc", NULL), 1, t_putchar);
			tputs(tgetstr("do", NULL), 1, t_putchar);
		}
		tputs(tgetstr("ce", NULL), 1, t_putchar);
		tputs(tgetstr("rc", NULL), 1, t_putchar);
	}
	(term->size)--;
}

void		handle_bs_key(t_term *term)
{
	if (term->cursor <= 0)
	{
		tputs(tgetstr("bl", NULL), 1, t_putchar);
		return ;
	}
	ft_memcpy(&(term->line[term->cursor - 1])
			, &(term->line[term->cursor])
			, term->size - term->cursor + 1);
	move_cursor_left(term);
	delete_char(term);
}

void		handle_del_key(t_term *term)
{
	if (term->cursor >= term->size)
	{
		tputs(tgetstr("bl", NULL), 1, t_putchar);
		return ;
	}
	ft_memmove(&(term->line[term->cursor])
			, &(term->line[term->cursor + 1])
			, term->size - term->cursor + 1);
	delete_char(term);
}
