/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 13:34:02 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/11 00:42:45 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include "shell.h"
#include "input.h"
#include "history.h"
#include "printf.h"

t_shell	*g_shell = NULL;

void	handle_signal(int sig)
{
	if (sig == SIGWINCH)
		update_winsize(&(g_shell->term));
	if (sig == SIGINT)
	{
		if (g_shell->term.visual_mode)
			vm_toggle(g_shell, &(g_shell->term));
		ft_putchar('\n');
		reset_cmdline(g_shell);
	}
}

int		init_shell(t_shell *shell, int argc, char **argv, char **environ)
{
	shell->argc = argc;
	shell->argv = argv;
	shell->env = parse_env(environ);
	shell->last_status = 0;
	shell->builtins = NULL;
	shell->history = NULL;
	shell->history_off = NULL;
	shell->term.capacity = 0;
	shell->term.size = 0;
	shell->term.line = NULL;
	shell->term.def_line = NULL;
	shell->term.ac_flag = 0;
	return (1);
}

int		main(int argc, char **argv, char **environ)
{
	t_shell	shell;

	g_shell = &shell;
	if (!init_shell(&shell, argc, argv, environ))
		return (1);
	signal(SIGINT, handle_signal);
	shell.term.legacy_mode = !setup_term(&shell);
	if (!(shell.term.legacy_mode))
	{
		signal(SIGWINCH, handle_signal);
		shell.term.seq_off = 0;
		shell.term.esc_seq = 0;
		shell.term.visual_mode = 0;
		shell.term.select.clipboard = NULL;
	}
	wait_for_command(&shell);
	reset_term(&shell);
	return (0);
}
