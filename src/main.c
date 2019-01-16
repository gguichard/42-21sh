/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 13:34:02 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/16 11:28:36 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include "shell.h"
#include "input.h"
#include "history.h"
#include "vars.h"
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
	ft_memset(shell, 0, sizeof(t_shell));
	shell->argc = argc;
	shell->argv = argv;
	shell->env = parse_env(environ);
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
		signal(SIGWINCH, handle_signal);
	wait_for_command(&shell);
	reset_term(&shell);
	free(shell.term.line);
	ft_lstdel(&(shell.env), free_var);
	ft_lstdel(&(shell.local), free_var);
	clear_history(&shell);
	return (0);
}
