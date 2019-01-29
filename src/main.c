/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 13:34:02 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/29 16:59:08 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "shell.h"
#include "input.h"
#include "history.h"
#include "vars.h"
#include "builtins.h"

t_shell				*g_shell = NULL;

static t_builtin	*build_builtin_tab(void)
{
	t_builtin	*new_tab;

	if ((new_tab = (t_builtin*)malloc(sizeof(t_builtin) * 7)) == NULL)
		return (NULL);
	new_tab[0].name = "echo";
	new_tab[0].builtin_fun = builtin_echo;
	new_tab[1].name = "cd";
	new_tab[1].builtin_fun = builtin_cd;
	new_tab[2].name = "exit";
	new_tab[2].builtin_fun = builtin_exit;
	new_tab[3].name = "env";
	new_tab[3].builtin_fun = builtin_env;
	new_tab[4].name = "unsetenv";
	new_tab[4].builtin_fun = builtin_unsetenv;
	new_tab[5].name = "setenv";
	new_tab[5].builtin_fun = builtin_setenv;
	new_tab[6].name = NULL;
	new_tab[6].builtin_fun = NULL;
	return (new_tab);
}

static int			init_shell(t_shell *shell, int argc, char **argv
		, char **environ)
{
	ft_memset(shell, 0, sizeof(t_shell));
	shell->argc = argc;
	shell->argv = argv;
	shell->env = parse_env(environ);
	shell->exec_hashtable = make_def_hashtable();
	if ((shell->builtins = build_builtin_tab()) == NULL)
		return (0);
	setup_signals();
	setup_def_vars(shell);
	update_winsize(&(shell->term));
	return (1);
}

void				destroy_shell(t_shell *shell)
{
	ft_lstdel(&(shell->env), free_var);
	ft_lstdel(&(shell->local), free_var);
	ft_strdel(&(shell->term.prev_lines));
	ft_strdel(&(shell->term.line));
	ft_strdel(&(shell->term.def_line));
	if (shell->exec_hashtable != NULL)
	{
		delete_hashtable(shell->exec_hashtable);
		shell->exec_hashtable = NULL;
	}
	ft_memdel((void**)&(shell->builtins));
	clear_history(shell);
}

int					main(int argc, char **argv, char **environ)
{
	t_shell	shell;
	int		ret;

	g_shell = &shell;
	ret = init_shell(&shell, argc, argv, environ);
	if (ret)
		wait_for_command(&shell);
	destroy_shell(&shell);
	return (!ret);
}
