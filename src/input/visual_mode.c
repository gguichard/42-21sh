/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visual_mode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 14:17:27 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/08 19:25:33 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "shell.h"
#include "input.h"
#include "utils.h"

void	vm_toggle(t_term *term)
{
	term->visual_mode = !(term->visual_mode);
	if (!(term->visual_mode))
		refresh_prompt_command(term);
	else
	{
		if (term->cursor == term->size && term->size > 0)
			term->cursor = term->size - 1;
		term->select.begin = term->cursor;
	}
}

void	vm_copy_hook(t_term *term, int cut)
{
	size_t	begin;
	size_t	end;
	size_t	off;

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
			term->size -= off;
		}
	}
}

void	vm_paste_hook(t_term *term, int before_cursor)
{
	char	*curr;

	curr = term->select.clipboard;
	if (curr == NULL)
		return ;
	if (term->select.begin != term->select.end)
	{
		tputs(tgetstr("bl", NULL), 1, t_putchar);
		return ;
	}
	term->visual_mode = 0;
	if (!before_cursor)
		move_cursor_right(term);
	while (*curr != '\0')
	{
		insert_cmdline(term, *curr);
		curr++;
	}
	term->visual_mode = 1;
}

int		handle_vm_key(t_term *term, char key)
{
	if (key != 'y' && key != 'd' && key != 'p' && key != 'P')
	{
		tputs(tgetstr("bl", NULL), 1, t_putchar);
		return (1);
	}
	if (key == 'y')
		vm_copy_hook(term, 0);
	else if (key == 'd')
		vm_copy_hook(term, 1);
	else if (key == 'p')
		vm_paste_hook(term, 0);
	else if (key == 'P')
		vm_paste_hook(term, 1);
	term->visual_mode = 0;
	term->cursor = term->select.begin;
	refresh_prompt_command(term);
	return (1);
}
