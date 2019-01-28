/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/28 15:20:59 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/28 15:40:13 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "shell.h"

int	builtin_echo(t_shell *shell, int argc, char **argv)
{
	int	index;
	int	print_nl;

	(void)shell;
	index = 1;
	print_nl = 1;
	if (argc > 1 && ft_strequ("-n", argv[1]))
	{
		print_nl = 0;
		index++;
	}
	while (index < argc)
	{
		ft_putstr(argv[index]);
		if (index + 1 < argc)
			ft_putchar(' ');
		index++;
	}
	if (print_nl)
		ft_putchar('\n');
	return (0);
}
