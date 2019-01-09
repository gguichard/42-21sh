/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_hooks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 10:16:58 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/09 14:20:37 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "input.h"
#include "history.h"
#include "utils.h"

static int	history_copy(t_shell *shell, t_term *term, const char *line)
{
	if (line == NULL)
	{
		tputs(tgetstr("bl", NULL), 1, t_putchar);
		return (0);
	}
	term->size = ft_strlen(line);
	term->cursor = term->size;
	ft_memcpy(term->line, line, term->size + 1);
	refresh_prompt_command(shell, term);
	return (1);
}

void		history_up(t_shell *shell, t_term *term)
{
	const char	*line;

	line = peek_history_prev(shell);
	if (line != NULL && term->def_line == NULL)
		term->def_line = ft_strdup(term->line);
	history_copy(shell, term, line);
}

void		history_down(t_shell *shell, t_term *term)
{
	const char	*line;
	int			should_free;

	line = peek_history_next(shell);
	should_free = 0;
	if (line == NULL && term->def_line != NULL)
	{
		line = term->def_line;
		term->def_line = NULL;
		should_free = 1;
	}
	history_copy(shell, term, line);
	if (should_free)
		free((char *)line);
}
