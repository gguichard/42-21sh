/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsetenv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 15:01:01 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/29 15:07:24 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "vars.h"
#include "builtins.h"

int	builtin_unsetenv(t_shell *shell, int argc, char **argv)
{
	int	index;

	if (argc <= 1)
	{
		ft_dprintf(2, "%s: usage: unsetenv name...\n", ERR_PREFIX, argv[0]);
		return (1);
	}
	index = 1;
	while (index < argc)
	{
		unset_var(&(shell->env), argv[index]);
		index++;
	}
	return (0);
}
