/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visual_mode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 14:17:27 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/08 15:45:03 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "input.h"
#include "utils.h"

void	vm_copy_hook(t_term *term, int cut)
{
	size_t	begin;
	size_t	end;
	size_t	off;

	begin = ft_min(term->select.end, term->select.begin);
	end = ft_max(term->select.end, term->select.begin);
	ft_strdel(&(term->select.clipboard));
	off = end - begin + 1;
	if (term->size >= off)
	{
		term->select.clipboard = ft_strsub(term->line, begin, off);
		if (cut)
		{
			ft_memcpy(&(term->line[begin]), &(term->line[end + 1])
					, term->size - end);
			term->size -= off;
			if (term->select.end > term->select.begin)
				term->cursor = term->select.begin;
			else
				term->cursor = ft_min(term->select.end, term->size);
		}
	}
}

void	vm_paste_hook(t_term *term)
{
	char	*curr;

	curr = term->select.clipboard;
	if (curr == NULL || term->select.begin != term->select.end)
	{
		tputs(tgetstr("bl", NULL), 1, t_putchar);
		return ;
	}
	while (*curr != '\0')
	{
		insert_cmdline(term, *curr);
		curr++;
	}
}

int		handle_vm_key(t_term *term, unsigned char key)
{
	if (key != 'c' && key != 'x' && key != 'v' && key != 'p')
	{
		tputs(tgetstr("bl", NULL), 1, t_putchar);
		return (1);
	}
	if (key == 'c')
		vm_copy_hook(term, 0);
	else if (key == 'x')
		vm_copy_hook(term, 1);
	else if (key == 'v' || key == 'p')
		vm_paste_hook(term);
	term->visual_mode = 0;
	refresh_prompt_command(term);
	return (1);
}
