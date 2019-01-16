/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 10:05:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/15 17:58:40 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "input.h"
#include "utils.h"

void	reset_cmdline(t_shell *shell)
{
	ft_memset(shell->term.line, 0, shell->term.size + 1);
	show_prompt(shell);
	shell->term.cursor = 0;
	shell->term.row = 0;
	shell->term.col = shell->term.offset;
	shell->term.rows = 1;
	shell->term.size = 0;
	shell->term.esc_seq = 0;
	shell->term.seq_off = 0;
}

int		realloc_cmdline(t_term *term)
{
	char	*tmp;

	tmp = (char *)ft_memalloc(term->capacity + CMDLINE_CAPACITY + 1);
	if (tmp == NULL)
		return (0);
	if (term->line != NULL)
		ft_memcpy(tmp, term->line, term->size);
	free(term->line);
	term->line = tmp;
	term->capacity += CMDLINE_CAPACITY;
	return (1);
}

void	insert_cmdline(t_shell *shell, t_term *term, char key)
{
	if (term->size < term->capacity || realloc_cmdline(term))
	{
		if (term->size > term->cursor)
		{
			ft_memmove(&(term->line[term->cursor + 1])
					, &(term->line[term->cursor])
					, term->size - term->cursor);
		}
		(term->size)++;
		(term->line)[term->size] = '\0';
		(term->line)[term->cursor] = key;
		move_cursor_right(shell, term);
		refresh_cmdline(shell, term);
	}
}

void	refresh_cmdline(t_shell *shell, t_term *term)
{
	tputs(tgetstr("cr", NULL), 1, t_putchar);
	if (term->row > 0)
		tputs(tparm(tgetstr("UP", NULL), term->row), 1, t_putchar);
	tputs(tgetstr("cd", NULL), 1, t_putchar);
	print_cmdline(shell, term);
}

void	print_cmdline(t_shell *shell, t_term *term)
{
	show_prompt(shell);
	tputs(tgetstr("sc", NULL), 1, t_putchar);
	(term->visual_mode)
		? print_select_line(term)
		: write(STDOUT_FILENO, term->line, term->size);
	tputs(tgetstr("rc", NULL), 1, t_putchar);
	update_cursor_data(term);
	if (term->row > 0)
		tputs(tparm(tgetstr("DO", NULL), term->row), 1, t_putchar);
	tputs(tparm(tgetstr("ch", NULL), term->col), 1, t_putchar);
}
