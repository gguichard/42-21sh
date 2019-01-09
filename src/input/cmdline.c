/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 10:05:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/09 15:46:47 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "input.h"
#include "utils.h"

void	reset_cmdline(t_shell *shell)
{
	ft_memset(shell->term.line, 0, shell->term.size + 1);
	shell->term.size = 0;
	shell->term.cursor = 0;
	shell->term.esc_seq = 0;
	shell->term.seq_off = 0;
	show_prompt(shell);
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
	move_cursor_right(shell, term);
	refresh_prompt_command(shell, term);
}

void	refresh_prompt_command(t_shell *shell, t_term *term)
{
	int	rows;
	int	curr;
	int	offset;

	tputs(tgetstr("cr", NULL), 1, t_putchar);
	rows = (term->cursor + term->offset) / term->winsize.ws_col;
	if (rows > 0)
		tputs(tparm(tgetstr("UP", NULL), rows), 1, t_putchar);
	tputs(tgetstr("cd", NULL), 1, t_putchar);
	show_prompt(shell);
	print_cmdline(term);
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

void	print_cmdline(t_term *term)
{
	size_t	select_begin;
	size_t	select_end;

	if (!(term->visual_mode))
	{
		write(STDOUT_FILENO, term->line, term->size);
		return ;
	}
	select_begin = ft_min(term->select.end, term->select.begin);
	select_end = ft_max(term->select.end, term->select.begin) + 1;
	if (select_begin > 0)
		write(STDOUT_FILENO, term->line, select_begin);
	if (select_end > select_begin)
	{
		tputs(tgetstr("mr", NULL), 1, t_putchar);
		write(STDOUT_FILENO, &(term->line[select_begin])
				, select_end - select_begin);
		tputs(tgetstr("me", NULL), 1, t_putchar);
	}
	if (select_end < term->size)
		write(STDOUT_FILENO, &(term->line[select_end])
				, term->size - select_end);
}
