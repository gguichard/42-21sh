/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 13:34:02 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/03 18:26:36 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

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
	return (0);
}
