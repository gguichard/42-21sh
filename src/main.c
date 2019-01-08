/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 13:34:02 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/08 13:33:25 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include "shell.h"
#include "input.h"

t_shell	*g_shell = NULL;

void	handle_signal(int sig)
{
	if (sig == SIGWINCH)
		update_winsize(&(g_shell->term));
}

int		main(int argc, char **argv, char **environ)
{
	t_shell	shell;

	g_shell = &shell;
	shell.argc = argc;
	shell.argv = argv;
	shell.env = parse_env(environ);
	shell.last_status = 0;
	shell.term.legacy_mode = !setup_term(&shell);
	if (!shell.term.legacy_mode)
	{
		shell.term.seq_off = 0;
		shell.term.esc_seq = 0;
	}
	shell.term.visual_mode = 0;
	update_winsize(&(shell.term));
	signal(SIGWINCH, handle_signal);
	wait_for_command(&shell);
	reset_term(&shell);
	return (0);
}
