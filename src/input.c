/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 21:25:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/04 10:27:02 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "get_next_line.h"
#include "shell.h"
#include "input.h"

int	exec_todo(t_shell *shell)
{
	char	*cmdline;

	cmdline = get_cmdline(shell);
	ft_printf("COMMAND: %s\n", cmdline);
	return (1);
}

int	read_input(t_shell *shell)
{
	char	buf;
	int		ret;

	while ((ret = read(STDIN_FILENO, &buf, 1)) > 0)
	{
		if (buf == 27)
			shell->term.esc_seq = !(shell->term.esc_seq);
		if (!(shell->term.esc_seq) || append_esc_seq(shell, buf) < 0)
		{
			ft_putchar(buf);
			if (buf == '\n')
				break ;
			append_char_cmdline(shell, buf);
		}
	}
	return (ret);
}

int	wait_for_command(t_shell *shell)
{
	int	ret;

	shell->term.line = NULL;
	shell->term.cmdline = NULL;
	shell->term.cmdline_size = 0;
	while (1)
	{
		if (!(shell->term.legacy_mode))
		{
			if ((ret = read_input(shell) <= 0))
				return (ret);
		}
		else
		{
			ret = get_next_line(0, &(shell->term.line));
			if (ret <= 0)
				return (ret);
		}
		exec_todo(shell);
	}
	return (1);
}
