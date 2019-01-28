/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 13:34:02 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/28 10:45:28 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"
#include "history.h"
#include "vars.h"

t_shell		*g_shell = NULL;

static int	init_shell(t_shell *shell, int argc, char **argv
		, char **environ)
{
	ft_memset(shell, 0, sizeof(t_shell));
	shell->argc = argc;
	shell->argv = argv;
	shell->env = parse_env(environ);
	shell->exec_hashtable = make_def_hashtable();
	setup_signals();
	return (1);
}

int			main(int argc, char **argv, char **environ)
{
	t_shell	shell;

	g_shell = &shell;
	if (!init_shell(&shell, argc, argv, environ))
		return (1);
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
