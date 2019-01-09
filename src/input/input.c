/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 21:25:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/09 12:30:49 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "get_next_line.h"
#include "shell.h"
#include "input.h"
#include "history.h"

int	handle_command(t_shell *shell)
{
	ft_printf("COMMAND: %s\n", shell->term.line);
	add_history_entry(shell, shell->term.line);
	return (1);
}

int	handle_key_mode(t_shell *shell, t_term *term, char key)
{
	if (term->visual_mode)
		return (handle_vm_key(shell, term, key));
	else
		return (handle_key(shell, term, key));
}

int	read_input(t_shell *shell)
{
	int		ret;
	char	buf;

	while ((ret = read(STDIN_FILENO, &buf, 1)) > 0)
	{
		if (buf == 4)
		{
			if (handle_eot_key(shell, &(shell->term)))
				return (0);
		}
		else
		{
			if (!handle_esc_key(shell, &(shell->term), buf)
					&& !handle_key_mode(shell, &(shell->term), buf))
				break ;
		}
	}
	return (ret);
}

int	wait_for_command(t_shell *shell)
{
	int	ret;

	shell->term.line = NULL;
	shell->term.capacity = 0;
	if (!realloc_cmdline(&(shell->term)))
		return (0);
	shell->term.size = shell->term.capacity;
	ret = 1;
	while (ret > 0)
	{
		ft_memset(shell->term.line, 0, shell->term.size + 1);
		shell->term.size = 0;
		shell->term.cursor = 0;
		show_prompt(shell);
		if (!(shell->term.legacy_mode))
			ret = read_input(shell);
		else
			ret = get_next_line(STDIN_FILENO, &(shell->term.line));
		if (ret > 0)
			handle_command(shell);
	}
	ft_putendl("exit");
	return (ret);
}
