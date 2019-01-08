/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 21:25:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/08 19:28:17 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "get_next_line.h"
#include "shell.h"
#include "input.h"

int	handle_command(t_shell *shell)
{
	ft_printf("COMMAND: %s\n", shell->term.line);
	ft_strdel(&(shell->term.line));
	return (1);
}

int	handle_key_mode(t_term *term, char key)
{
	if (term->visual_mode)
		return (handle_vm_key(term, key));
	else
		return (handle_key(term, key));
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
					&& !handle_key_mode(&(shell->term), buf))
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
			ret = read_input(shell);
		else
			ret = get_next_line(STDIN_FILENO, &(shell->term.line));
		if (ret <= 0)
			break ;
		handle_command(shell);
	}
	ft_putendl("exit");
	return (ret);
}
