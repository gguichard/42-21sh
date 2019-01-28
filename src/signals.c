/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/28 10:03:17 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/28 13:52:03 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <signal.h>
#include "shell.h"
#include "input.h"

extern t_shell	*g_shell;

static void		clean_exit(int sig)
{
	reset_term(g_shell);
	exit(sig);
}

void			kill_forked_pids(t_shell *shell)
{
	t_list	*curr;

	curr = shell->fork_pids;
	while (curr != NULL)
	{
		kill(*((pid_t *)curr->content), SIGINT);
		curr = curr->next;
	}
}

static void		handle_signal(int sig)
{
	if (sig == SIGWINCH)
		update_winsize(&(g_shell->term));
	if (sig == SIGINT)
	{
		if (g_shell->fork_pids != NULL)
		{
			kill_forked_pids(g_shell);
			ft_putchar('\n');
		}
		else
		{
			move_cursor_end(g_shell, &(g_shell->term));
			ft_putchar('\n');
			ft_strdel(&(g_shell->term.prev_lines));
			if (g_shell->term.visual_mode)
				vm_toggle(g_shell, &(g_shell->term));
			reset_cmdline(g_shell, PROMPT_DEF);
		}
	}
}

void			setup_signals(void)
{
	signal(SIGHUP, clean_exit);
	signal(SIGTERM, clean_exit);
	signal(SIGINT, clean_exit);
	signal(SIGQUIT, clean_exit);
	signal(SIGCONT, clean_exit);
	signal(SIGWINCH, handle_signal);
	signal(SIGINT, handle_signal);
	signal(SIGTSTP, handle_signal);
}
