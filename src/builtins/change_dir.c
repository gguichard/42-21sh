/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_dir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/23 15:13:49 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/23 16:26:22 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "shell.h"
#include "options.h"

int	builtin_cd(t_shell *shell, int argc, char **argv)
{
	t_opts	*opts;
	int		index;

	(void)shell;
	opts = parse_opts(argc, argv, "LP");
	if (opts == NULL)
	{
		ft_dprintf(2, "%s: %s: unexpected error\n", ERR_PREFIX, argv[0]);
		return (1);
	}
	else if (opts->error != 0)
	{
		ft_dprintf(2, "%s: %s: -%c: invalid option\n", ERR_PREFIX, argv[0]
				, opts->error);
		ft_dprintf(2, "%s: usage: cd [-L|-P] [dir]\n", argv[0]);
		free(opts);
		return (1);
	}
	index = opts->index;
	while (index < argc)
	{
		ft_printf("ARG: %s\n", argv[index]);
		index++;
	}
	free(opts);
	return (0);
}
