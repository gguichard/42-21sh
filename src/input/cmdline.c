/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 10:05:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/15 10:44:15 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "input.h"
#include "utils.h"

size_t	get_rows(t_term *term)
{
	size_t	index;
	size_t	rows;
	size_t	cursor;

	index = 0;
	rows = 1;
	cursor = term->offset;
	while (index < term->size)
	{
		if ((term->line)[index] != '\n'
				&& (cursor + 1) % term->winsize.ws_col != 0)
			cursor++;
		else
		{
			cursor = 0;
			rows++;
		}
		index++;
	}
	return (rows);
}

size_t	get_max_col(t_term *term)
{
	if (!ft_strchr(term->line, '\n'))
	{
		if ((term->row + 1) < term->rows)
			return (term->winsize.ws_col - 1);
		return ((term->size + term->offset) % term->winsize.ws_col);
	}
	return (0);
}

void	reset_cmdline(t_shell *shell)
{
	show_prompt(shell);
	ft_memset(shell->term.line, 0, shell->term.size + 1);
	shell->term.esc_seq = 0;
	shell->term.seq_off = 0;
	shell->term.size = 0;
	shell->term.cursor = 0;
	shell->term.row = 0;
	shell->term.col = shell->term.offset;
	shell->term.rows = 1;
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
	if (term->size == term->capacity && !realloc_cmdline(term))
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
	term->rows = get_rows(term);
	move_cursor_right(shell, term);
	refresh_cmdline(shell, term);
}

void	refresh_cmdline(t_shell *shell, t_term *term)
{
	tputs(tgetstr("cr", NULL), 1, t_putchar);
	if (term->row > 0)
		tputs(tparm(tgetstr("UP", NULL), term->row), 1, t_putchar);
	tputs(tgetstr("cd", NULL), 1, t_putchar);
	print_cmdline(shell, term);
}

void	print_select_line(t_term *term)
{
	size_t	sl_begin;
	size_t	sl_end;

	sl_begin = ft_min(term->select.end, term->select.begin);
	sl_end = ft_max(term->select.end, term->select.begin) + 1;
	if (sl_begin > 0)
		write(STDOUT_FILENO, term->line, sl_begin);
	if (sl_end > sl_begin)
	{
		tputs(tgetstr("mr", NULL), 1, t_putchar);
		write(STDOUT_FILENO, &(term->line[sl_begin]), sl_end - sl_begin);
		tputs(tgetstr("me", NULL), 1, t_putchar);
	}
	if (sl_end < term->size)
		write(STDOUT_FILENO, &(term->line[sl_end]), term->size - sl_end);
}

void	print_cmdline(t_shell *shell, t_term *term)
{
	show_prompt(shell);
	tputs(tgetstr("sc", NULL), 1, t_putchar);
	(term->visual_mode)
		? print_select_line(term)
		: write(STDOUT_FILENO, term->line, term->size);
	tputs(tgetstr("rc", NULL), 1, t_putchar);
	if (term->row > 0)
		tputs(tparm(tgetstr("DO", NULL), term->row), 1, t_putchar);
	tputs(tparm(tgetstr("ch", NULL), term->col), 1, t_putchar);
}
