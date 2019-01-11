/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visual_mode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 14:17:27 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/11 11:22:05 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "shell.h"
#include "input.h"
#include "utils.h"

int	vm_toggle(t_shell *shell, t_term *term)
{
	term->visual_mode = !(term->visual_mode);
	if (!(term->visual_mode))
		refresh_cmdline(shell, term);
	else
	{
		if (term->size > 0 && term->cursor >= term->size)
			term->cursor = term->size - 1;
		term->select.begin = term->cursor;
	}
	return (1);
}

int	vm_copy_hook(t_shell *shell, t_term *term, int cut)
{
	size_t	begin;
	size_t	end;
	size_t	off;

	(void)shell;
	ft_strdel(&(term->select.clipboard));
	begin = ft_min(term->select.end, term->select.begin);
	end = ft_max(term->select.end, term->select.begin);
	off = end - begin + 1;
	if (term->size >= off)
	{
		term->select.clipboard = ft_strsub(term->line, begin, off);
		if (cut)
		{
			ft_memcpy(&(term->line[begin]), &(term->line[end + 1])
					, term->size - end);
			term->cursor = begin;
			term->size -= off;
		}
	}
	return (1);
}

int	vm_paste_hook(t_shell *shell, t_term *term, int before_cursor)
{
	char	*curr;

	curr = term->select.clipboard;
	if (curr == NULL || term->select.begin != term->select.end)
		return (0);
	term->visual_mode = 0;
	if (!before_cursor)
		move_cursor_right(shell, term);
	while (*curr != '\0')
	{
		insert_cmdline(shell, term, *curr);
		curr++;
	}
	term->visual_mode = 1;
	term->select.end = term->cursor;
	return (1);
}

int	handle_vm_key(t_shell *shell, t_term *term, char key)
{
	int	ret;

	ret = 0;
	if (key == 'y')
		ret = vm_copy_hook(shell, term, 0);
	else if (key == 'd')
		ret = vm_copy_hook(shell, term, 1);
	else if (key == 'p')
		ret = vm_paste_hook(shell, term, 0);
	else if (key == 'P')
		ret = vm_paste_hook(shell, term, 1);
	if (!ret)
	{
		tputs(tgetstr("bl", NULL), 1, t_putchar);
		return (1);
	}
	vm_toggle(shell, term);
	return (1);
}
