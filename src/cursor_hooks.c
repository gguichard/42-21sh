/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 12:18:37 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/04 12:20:38 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "utils.h"

void	move_cursor_left(t_shell *shell)
{
	if (shell->term.cmdline_curr && !(shell->term.cmdline_curr->prev))
		return ;
	if (shell->term.cmdline_curr == NULL)
		shell->term.cmdline_curr = shell->term.cmdline;
	else if (shell->term.cmdline_curr->prev != NULL)
		shell->term.cmdline_curr = shell->term.cmdline_curr->prev;
	tputs(tgetstr("le", NULL), 1, term_putchar);
}
