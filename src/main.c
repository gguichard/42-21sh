/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 13:34:02 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/03 23:18:36 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"

int	main(int argc, char **argv, char **environ)
{
	t_shell	shell;

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
	wait_for_command(&shell);
	reset_term(&shell);
	return (0);
}
