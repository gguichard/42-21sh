/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 17:10:37 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/20 13:54:07 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "shell.h"

void	show_prompt(t_shell *shell)
{
	char	*prompt;
	char	*tmp;

	if (shell->term.prompt == PROMPT_ESCAPED)
		prompt = ft_strdup("> ");
	else if (shell->term.prompt == PROMPT_QUOTE)
		prompt = ft_strdup("quote> ");
	else if (shell->term.prompt == PROMPT_DQUOTE)
		prompt = ft_strdup("dquote> ");
	else if (shell->term.prompt == PROMPT_HEREDOC)
		prompt = ft_strdup("heredoc> ");
	else
		prompt = ft_strdup("$> ");
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
