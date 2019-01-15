/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 16:06:26 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/15 11:42:37 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"
#include "utils.h"

static void	del_on_cursor(t_shell *shell, t_term *term)
{
	ft_memmove(&(term->line[term->cursor])
			, &(term->line[term->cursor + 1])
			, term->size - term->cursor + 1);
	(term->size)--;
	term->rows = get_rows(term);
	refresh_cmdline(shell, term);
}

int			handle_bs_key(t_shell *shell, t_term *term)
{
	if (!move_cursor_left(shell, term))
		return (0);
	del_on_cursor(shell, term);
	return (1);
}

int			handle_del_key(t_shell *shell, t_term *term)
{
	if (term->row + 1 == term->rows
			&& term->col == get_max_col(term))
		return (0);
	del_on_cursor(shell, term);
	return (1);
}
