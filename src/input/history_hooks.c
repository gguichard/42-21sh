/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_hooks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 10:16:58 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/16 10:48:55 by gguichar         ###   ########.fr       */
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
	size_t	len;

	if (line == NULL)
		return (0);
	len = ft_strlen(line);
	ft_memcpy(term->line, line, len + 1);
	term->size = len;
	term->cursor = len;
	refresh_cmdline(shell, term);
	return (1);
}

int			history_up(t_shell *shell, t_term *term)
{
	const char	*line;

	line = peek_history_prev(shell);
	if (line != NULL && term->def_line == NULL)
		term->def_line = ft_strdup(term->line);
	return (history_copy(shell, term, line));
}

int			history_down(t_shell *shell, t_term *term)
{
	const char	*line;
	int			should_free;
	int			ret;

	line = peek_history_next(shell);
	should_free = 0;
	if (line == NULL && term->def_line != NULL)
	{
		line = term->def_line;
		term->def_line = NULL;
		should_free = 1;
	}
	ret = history_copy(shell, term, line);
	if (should_free)
		free((char *)line);
	return (ret);
}
