/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/25 14:20:19 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/27 17:38:40 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "get_next_line.h"
#include "shell.h"
#include "input.h"
#include "redirect_inf.h"

void		prompt_heredoc(t_shell *shell, t_redirect_inf *redirect_inf)
{
	int	ret;

	ret = 1;
	while (ret > 0)
	{
		reset_cmdline(shell, PROMPT_HEREDOC);
		if ((ret = read_input(shell)) <= 0)
			ft_strdel(&(redirect_inf->heredoc));
		else
		{
			redirect_inf->heredoc = get_command_line(&(shell->term));
			free(shell->term.prev_lines);
			if (ft_strequ(shell->term.line, redirect_inf->to_word))
				break ;
			shell->term.prev_lines = redirect_inf->heredoc;
		}
	}
	if (redirect_inf->heredoc == NULL)
		ft_dprintf(2, "%s: Wrong heredoc end\n", ERR_PREFIX);
	else
		(redirect_inf->heredoc)[ft_strlen(redirect_inf->heredoc)
			- ft_strlen(redirect_inf->to_word)] = '\0';
	shell->term.prev_lines = NULL;
	shell->term.prompt = PROMPT_DEF;
}

int			redirect_heredoc(t_redirect_inf *redirect_inf)
{
	int	fildes[2];

	if (redirect_inf->heredoc == NULL)
		return (0);
	if (pipe(fildes) < 0)
	{
		ft_dprintf(2, "%s: heredoc: Pipe error\n", ERR_PREFIX);
		return (0);
	}
	dup2(fildes[0], STDIN_FILENO);
	write(fildes[1], redirect_inf->heredoc
			, ft_strlen(redirect_inf->heredoc));
	close(fildes[1]);
	return (1);
}
