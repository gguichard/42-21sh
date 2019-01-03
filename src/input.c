/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 21:25:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/03 23:34:01 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <unistd.h>
#include "get_next_line.h"
#include "shell.h"
#include "input.h"

int	exec_todo(t_shell *shell)
{
	ft_printf("would execute: %s\n", shell->term.line);
	return (1);
}

int	is_valid_esc_seq(t_shell *shell)
{
	static char	*seqs[] = {
		ESC_SEQ_LEFT,
		NULL
	};
	int			index;

	index = 0;
	while (seqs[index] != NULL)
	{
		if (ft_strnequ(shell->term.seq, seqs[index], shell->term.seq_off))
			return (ft_strlen(seqs[index]) == (size_t)shell->term.seq_off);
		index++;
	}
	return (-1);
}

int	append_esc_seq(t_shell *shell, char key)
{
	int	ret;

	(shell->term.seq)[shell->term.seq_off] = key;
	(shell->term.seq_off)++;
	ret = is_valid_esc_seq(shell);
	if (ret == 1)
		ft_printf("got good esc seq\n");
	if (ret != 0 || shell->term.seq_off == MAX_ESC_SEQ_BYTES)
	{
		shell->term.esc_seq = 0;
		shell->term.seq_off = 0;
		return (ret);
	}
	return (0);
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
			if (buf == '\n')
				break ;
			ft_putchar(buf);
		}
	}
	return (ret);
}

int	wait_for_command(t_shell *shell)
{
	int	ret;

	shell->term.line = NULL;
	shell->term.line_off = 0;
	while (1)
	{
		if (!shell->term.legacy_mode)
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
