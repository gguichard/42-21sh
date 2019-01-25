/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/25 14:20:19 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/25 16:43:40 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "get_next_line.h"
#include "shell.h"
#include "input.h"
#include "redirect_inf.h"

static void	reset_heredoc_cmd(t_shell *shell)
{
	if (shell->term.line != NULL)
		ft_memset(shell->term.line, 0, shell->term.size + 1);
	shell->term.prompt = PROMPT_HEREDOC;
	show_prompt(shell);
	shell->term.cursor = 0;
	shell->term.size = 0;
	shell->term.row = 0;
	shell->term.col = shell->term.offset;
	shell->term.rows = 1;
	shell->term.esc_seq = 0;
	shell->term.seq_off = 0;
}

static int	read_heredoc_input(t_shell *shell)
{
	int	ret;

	if (!(shell->term.legacy_mode))
		ret = read_input(shell);
	else
	{
		ret = get_next_line(STDIN_FILENO, &(shell->term.line));
		if (ret > 0)
			shell->term.size = ft_strlen(shell->term.line);
	}
	return (ret);
}

void		prompt_heredoc(t_shell *shell, t_redirect_inf *redirect_inf)
{
	int	ret;

	ret = 1;
	while (ret > 0)
	{
		reset_heredoc_cmd(shell);
		if ((ret = read_heredoc_input(shell)) <= 0)
			break ;
		redirect_inf->heredoc = get_command_line(&(shell->term));
		if (ft_strequ(shell->term.line, redirect_inf->to_word))
			break ;
		free(shell->term.multiline);
		shell->term.multiline = redirect_inf->heredoc;
	}
	if (redirect_inf->heredoc == NULL)
		ft_dprintf(2, "%s: %s: Bad end of file\n", ERR_PREFIX
				, redirect_inf->to_word);
	else
		(redirect_inf->heredoc)[ft_strlen(redirect_inf->heredoc)
			- ft_strlen(redirect_inf->to_word)] = '\0';
	shell->term.multiline = NULL;
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
