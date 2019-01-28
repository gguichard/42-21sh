/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/28 14:13:52 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/28 17:16:55 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "builtins.h"

int	builtin_exit(t_shell *shell, int argc, char **argv)
{
	int		status;
	char	*endptr;

	status = 0;
	if (argc > 2)
		ft_dprintf(2, "%s: %s: too many arguments.\n");
	else
	{
		if (argc > 1)
		{
			status = ft_strtol(argv[1], &endptr, 10);
			if (*endptr != '\0')
			{
				status = 255;
				ft_dprintf(2, "%s: %s: %s: numeric argument required\n"
						, ERR_PREFIX, argv[0], argv[1]);
			}
		}
		destroy_shell(shell);
		exit(status);
	}
	return (1);
}
