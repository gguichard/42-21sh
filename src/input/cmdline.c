/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 10:05:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/08 01:41:52 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "input.h"
#include "utils.h"

extern t_shell	*g_shell;

void			print_cmdline(t_term *term)
{
	int	rows;
	int	curr;
	int	offset;

	tputs(tgetstr("cr", NULL), 1, t_putchar);
	rows = (term->cursor + term->offset) / term->winsize.ws_col;
	if (rows > 0)
		tputs(tparm(tgetstr("UP", NULL), rows), 1, t_putchar);
	tputs(tgetstr("cd", NULL), 1, t_putchar);
	show_prompt(g_shell);
	ft_putstr(term->line);
	if ((term->size + term->offset) % term->winsize.ws_col == 0)
	{
		tputs(tgetstr("cr", NULL), 1, t_putchar);
		tputs(tgetstr("do", NULL), 1, t_putchar);
	}
	if (term->cursor < term->size)
	{
		rows = (term->size + term->offset) / term->winsize.ws_col;
		curr = (term->cursor + term->offset) / term->winsize.ws_col;
		if (curr < rows)
			tputs(tparm(tgetstr("UP", NULL), rows - curr), 1, t_putchar);
		offset = (term->cursor + term->offset) % term->winsize.ws_col;
		tputs(tparm(tgetstr("ch", NULL), offset), 1, t_putchar);
	}
}

int				realloc_cmdline(t_term *term)
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

void			append_cmdline(t_term *term, char key)
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
	move_cursor_right(term);
	print_cmdline(term);
}

int				handle_key(t_term *term, char key)
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
