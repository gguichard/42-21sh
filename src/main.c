/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 13:34:02 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/28 09:52:33 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "shell.h"
#include "input.h"
#include "history.h"
#include "vars.h"

t_shell			*g_shell = NULL;

static void		kill_forked_pids(t_shell *shell)
{
	t_list	*curr;

	if (shell->fork_pids != NULL)
	{
		curr = shell->fork_pids;
		while (curr != NULL)
		{
			kill(*((pid_t *)curr->content), SIGINT);
			curr = curr->next;
		}
	}
}

void			handle_signal(int sig)
{
	if (sig == SIGWINCH)
		update_winsize(&(g_shell->term));
	if (sig == SIGINT)
	{
		kill_forked_pids(g_shell);
		move_cursor_end(g_shell, &(g_shell->term));
		ft_putchar('\n');
		if (g_shell->fork_pids == NULL)
		{
			ft_strdel(&(g_shell->term.prev_lines));
			if (g_shell->term.visual_mode)
				vm_toggle(g_shell, &(g_shell->term));
			reset_cmdline(g_shell, PROMPT_DEF);
		}
	}
}

int				init_shell(t_shell *shell, int argc, char **argv
		, char **environ)
{
	ft_memset(shell, 0, sizeof(t_shell));
	shell->argc = argc;
	shell->argv = argv;
	shell->env = parse_env(environ);
	shell->exec_hashtable = make_def_hashtable();
	return (1);
}

int				main(int argc, char **argv, char **environ)
{
	t_shell	shell;

	g_shell = &shell;
	if (!init_shell(&shell, argc, argv, environ))
		return (1);
	signal(SIGINT, handle_signal);
	signal(SIGWINCH, handle_signal);
	wait_for_command(&shell);
	ft_lstdel(&(shell.env), free_var);
	ft_lstdel(&(shell.local), free_var);
	ft_strdel(&(shell.term.prev_lines));
	ft_strdel(&(shell.term.line));
	ft_strdel(&(shell.term.def_line));
	if (shell.exec_hashtable != NULL)
	{
		delete_hashtable(shell.exec_hashtable);
		shell.exec_hashtable = NULL;
	}
	clear_history(&shell);
	return (0);
}
