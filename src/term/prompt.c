/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 17:10:37 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/29 00:08:17 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "shell.h"
#include "vars.h"

static char	*get_default_prompt(t_shell *shell)
{
	char	*prompt;
	char	*tmp;

	prompt = ft_strjoin_free(get_shell_var(shell, "USER"), ":");
	tmp = get_shell_var(shell, "PWD");
	prompt = ft_strjoin_free(prompt, tmp);
	free(tmp);
	prompt = ft_strjoin_free(prompt, "% ");
	return (prompt);
}

static char	*get_prompt_with_type(t_shell *shell)
{
	char	*prompt;

	prompt = NULL;
	if (shell->term.prompt == PROMPT_ESCAPED)
		prompt = ft_strdup("> ");
	else if (shell->term.prompt == PROMPT_QUOTE)
		prompt = ft_strdup("quote> ");
	else if (shell->term.prompt == PROMPT_DQUOTE)
		prompt = ft_strdup("dquote> ");
	else if (shell->term.prompt == PROMPT_HEREDOC)
		prompt = ft_strdup("heredoc> ");
	else if (shell->term.prompt == PROMPT_BRACKET)
		prompt = ft_strdup("bracket> ");
	else if (shell->term.prompt == PROMPT_OPE)
		prompt = ft_strdup("pipe> ");
	return (prompt);
}

void		show_prompt(t_shell *shell)
{
	char	*prompt;
	char	*tmp;

	prompt = (shell->term.prompt == PROMPT_DEF) ? get_default_prompt(shell)
		: get_prompt_with_type(shell);
	if (shell->term.visual_mode)
	{
		tmp = prompt;
		prompt = ft_strjoin("(visual) ", prompt);
		free(tmp);
	}
	if (prompt == NULL)
		shell->term.offset = 0;
	else
	{
		shell->term.offset = write(STDOUT_FILENO, prompt, ft_strlen(prompt));
		free(prompt);
	}
}
