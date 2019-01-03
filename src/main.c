/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 13:34:02 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/03 21:01:08 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"

int	main(int argc, char **argv, char **environ)
{
	t_shell	shell;

	shell.argc = argc;
	shell.argv = argv;
	if ((shell.env = parse_env(environ)) == NULL)
	{
		ft_dprintf(2, "21sh: unable to parse env\n");
		return (1);
	}
	shell.last_status = 0;
	shell.legacy_mode = !setup_term(&shell);
	if (!shell.legacy_mode)
		reset_term(&shell);
	return (0);
}
