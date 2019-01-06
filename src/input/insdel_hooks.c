/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insdel_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 16:06:26 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/06 02:40:13 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"
#include "utils.h"

static void	delete_char(t_term *term)
{
	(void)term;
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
