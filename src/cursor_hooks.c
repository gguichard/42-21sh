/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 12:18:37 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/04 17:21:55 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "utils.h"

void	move_cursor_left(t_term *term)
{
	if (term->cursor <= 0)
		return ;
	(term->cursor)--;
	tputs(tgetstr("le", NULL), 1, term_putchar);
}

void	move_cursor_right(t_term *term)
{
	if (term->cursor >= ft_strlen(term->cmdline))
		return ;
	(term->cursor)++;
	tputs(tgetstr("nd", NULL), 1, term_putchar);
}
