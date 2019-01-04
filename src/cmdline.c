/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 10:05:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/04 17:26:54 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "input.h"
#include "utils.h"

void	print_cmdline(t_shell *shell, char key)
{
	ft_putchar(key);
	tputs(tgetstr("sc", NULL), 1, term_putchar);
	ft_putstr(&(shell->term.cmdline[shell->term.cursor]));
	tputs(tgetstr("rc", NULL), 1, term_putchar);
}

void	realloc_cmdline(t_shell *shell)
{
	char	*tmp;

	if (shell->term.cmdline_siz >= shell->term.cmdline_cap)
	{
		tmp = (char *)malloc(shell->term.cmdline_cap + CMDLINE_CAPACITY + 1);
		if (tmp == NULL)
			return ;
		ft_memcpy(tmp, shell->term.cmdline, shell->term.cmdline_siz);
		free(shell->term.cmdline);
		shell->term.cmdline = tmp;
		shell->term.cmdline_cap += CMDLINE_CAPACITY;
	}
}

void	append_char_cmdline(t_shell *shell, char key)
{
	size_t	off;

	realloc_cmdline(shell);
	off = shell->term.cmdline_siz - shell->term.cursor;
	if (off > 0)
	{
		ft_memmove(&(shell->term.cmdline[shell->term.cursor + 1])
				, &(shell->term.cmdline[shell->term.cursor])
				, off);
	}
	(shell->term.cmdline_siz)++;
	(shell->term.cmdline)[shell->term.cmdline_siz] = '\0';
	(shell->term.cmdline)[shell->term.cursor] = key;
	(shell->term.cursor)++;
	print_cmdline(shell, key);
}

void	handle_esc_key(t_shell *shell, const char *seq)
{
	if (ft_strequ(seq, ESC_SEQ_LEFT))
		move_cursor_left(&(shell->term));
	else if (ft_strequ(seq, ESC_SEQ_RIGHT))
		move_cursor_right(&(shell->term));
	else if (ft_strequ(seq, ESC_DEL_KEY))
		handle_del_key(&(shell->term));
}
