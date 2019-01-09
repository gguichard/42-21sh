/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_hooks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 10:16:58 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/09 10:22:54 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"
#include "history.h"
#include "utils.h"

extern t_shell	*g_shell;

void			history_up(t_term *term)
{
	const char	*line;

	line = peek_history_prev(g_shell);
	if (line == NULL)
	{
		tputs(tgetstr("bl", NULL), 1, t_putchar);
		return ;
	}
	term->size = ft_strlen(line);
	term->cursor = term->size;
	ft_memcpy(term->line, line, term->size + 1);
	refresh_prompt_command(term);
}

void			history_down(t_term *term)
{
	const char	*line;

	line = peek_history_next(g_shell);
	if (line == NULL)
	{
		tputs(tgetstr("bl", NULL), 1, t_putchar);
		return ;
	}
	term->size = ft_strlen(line);
	term->cursor = term->size;
	ft_memcpy(term->line, line, term->size + 1);
	refresh_prompt_command(term);
}
