/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 10:05:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/06 02:05:13 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "input.h"
#include "utils.h"

void	refresh_cmdline(t_term *term)
{
	if (term->cursor < term->size)
	{
		tputs(tgetstr("sc", NULL), 1, t_putchar);
		ft_putstr(&(term->line[term->cursor]));
		tputs(tgetstr("rc", NULL), 1, t_putchar);
	}
}

void	print_cmdline(t_term *term, char key)
{
	ft_putchar(key);
	if ((term->cursor + term->offset) % term->winsize.ws_col == 0)
	{
		tputs(tgetstr("cr", NULL), 1, t_putchar);
		tputs(tgetstr("sf", NULL), 1, t_putchar);
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
	if (term->size >= term->capacity && !realloc_cmdline(term))
		return ;
	if (term->size > term->cursor)
	{
		ft_memmove(&(term->line[term->cursor + 1])
				, &(term->line[term->cursor])
				, term->size - term->cursor);
	}
	(term->size)++;
	(term->line)[term->size] = '\0';
	(term->line)[term->cursor] = key;
	(term->cursor)++;
	print_cmdline(term, key);
}

int		handle_key(t_term *term, char key)
{
	if (key == '\n')
	{
		move_cursor_end(term);
		ft_putchar('\n');
		return (0);
	}
	if (key == 127)
	{
		handle_bs_key(term);
		return (1);
	}
	append_cmdline(term, key);
	return (1);
}
