/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_hooks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 10:16:58 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/09 12:24:11 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"
#include "history.h"
#include "utils.h"

static void	history_both(t_shell *shell, t_term *term, const char *line)
{
	if (line == NULL)
	{
		tputs(tgetstr("bl", NULL), 1, t_putchar);
		return ;
	}
	term->size = ft_strlen(line);
	term->cursor = term->size;
	ft_memcpy(term->line, line, term->size + 1);
	refresh_prompt_command(shell, term);
}

void		history_up(t_shell *shell, t_term *term)
{
	history_both(shell, term, peek_history_prev(shell));
}

void		history_down(t_shell *shell, t_term *term)
{
	history_both(shell, term, peek_history_next(shell));
}
