/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insdel_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 16:06:26 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/04 16:07:17 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "utils.h"

void	handle_bs_key(t_shell *shell)
{
	if (shell->term.cursor <= 0)
	{
		tputs(tgetstr("bl", NULL), 1, term_putchar);
		return ;
	}
	ft_memcpy(&(shell->term.cmdline[shell->term.cursor - 1])
			, &(shell->term.cmdline[shell->term.cursor])
			, shell->term.cmdline_siz - shell->term.cursor + 1);
	(shell->term.cursor)--;
	(shell->term.cmdline_siz)--;
	tputs(tgetstr("le", NULL), 1, term_putchar);
	tputs(tgetstr("dc", NULL), 1, term_putchar);
}

void	handle_del_key(t_shell *shell)
{
	if (shell->term.cursor >= shell->term.cmdline_siz)
	{
		tputs(tgetstr("bl", NULL), 1, term_putchar);
		return ;
	}
	ft_memmove(&(shell->term.cmdline[shell->term.cursor])
			, &(shell->term.cmdline[shell->term.cursor + 1])
			, shell->term.cmdline_siz - shell->term.cursor + 1);
	(shell->term.cmdline_siz)--;
	tputs(tgetstr("dc", NULL), 1, term_putchar);
}
