/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 12:18:37 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/04 16:07:05 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "utils.h"

void	move_cursor_left(t_shell *shell)
{
	if (shell->term.cursor <= 0)
		return ;
	(shell->term.cursor)--;
	tputs(tgetstr("le", NULL), 1, term_putchar);
}

void	move_cursor_right(t_shell *shell)
{
	if (shell->term.cursor >= ft_strlen(shell->term.cmdline))
		return ;
	(shell->term.cursor)++;
	tputs(tgetstr("nd", NULL), 1, term_putchar);
}
