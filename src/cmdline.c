/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 10:05:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/04 23:20:10 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "input.h"
#include "utils.h"

void	refresh_cmdline(t_term *term)
{
	if (term->cursor < term->size)
	{
		tputs(tgetstr("sc", NULL), 1, term_putchar);
		ft_putstr(&(term->line[term->cursor]));
		tputs(tgetstr("rc", NULL), 1, term_putchar);
	}
}

void	print_cmdline(t_term *term, char key)
{
	ft_putchar(key);
	if (term->cursor % term->winsize.ws_col == 0)
	{
		tputs(tgetstr("cr", NULL), 1, term_putchar);
		tputs(tgetstr("sf", NULL), 1, term_putchar);
	}
	refresh_cmdline(term);
}

int		realloc_cmdline(t_term *term)
{
	char	*tmp;

	tmp = (char *)malloc(term->capacity + CMDLINE_CAPACITY + 1);
	if (tmp == NULL)
		return (0);
	ft_memcpy(tmp, term->line, term->size);
	free(term->line);
	term->line = tmp;
	term->capacity += CMDLINE_CAPACITY;
	return (1);
}

void	append_cmdline(t_term *term, char key)
{
	size_t	off;

	if (term->size >= term->capacity && !realloc_cmdline(term))
		return ;
	off = term->size - term->cursor;
	if (off > 0)
	{
		ft_memmove(&(term->line[term->cursor + 1])
				, &(term->line[term->cursor])
				, off);
	}
	(term->size)++;
	(term->line)[term->size] = '\0';
	(term->line)[term->cursor] = key;
	(term->cursor)++;
	print_cmdline(term, key);
}

void	handle_esc_key(t_term *term, const char *seq)
{
	if (ft_strequ(seq, ESC_SEQ_LEFT))
		move_cursor_left(term);
	else if (ft_strequ(seq, ESC_SEQ_RIGHT))
		move_cursor_right(term);
	else if (ft_strequ(seq, ESC_DEL_KEY))
		handle_del_key(term);
}
