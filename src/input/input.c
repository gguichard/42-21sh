/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 21:25:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/07 19:12:18 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"
#include "shell.h"
#include "input.h"

int	handle_command(t_shell *shell)
{
	ft_printf("COMMAND: %s\n", shell->term.line);
	free(shell->term.line);
	return (1);
}

int	read_input(t_shell *shell)
{
	int		ret;
	char	buf;

	while ((ret = read(STDIN_FILENO, &buf, 1)) > 0)
	{
		if (buf == 4)
		{
			if (handle_eot_key(&(shell->term)))
				return (0);
		}
		else
		{
			if (!handle_esc_key(&(shell->term), buf)
					&& !handle_key(&(shell->term), buf))
				break ;
		}
	}
	return (ret);
}

int	wait_for_command(t_shell *shell)
{
	int	ret;

	ret = 1;
	while (setup_cmdline(&(shell->term)))
	{
		show_prompt(shell);
		if (!(shell->term.legacy_mode))
		{
			if ((ret = read_input(shell)) <= 0)
				break ;
		}
		else
		{
			ret = get_next_line(0, &(shell->term.line));
			if (ret <= 0)
				break ;
		}
		handle_command(shell);
	}
	ft_putendl("exit");
	return (ret);
}
