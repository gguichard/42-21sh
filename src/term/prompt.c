/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 17:10:37 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/29 16:44:25 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "shell.h"
#include "vars.h"

static size_t	print_def_prompt(t_shell *shell)
{
	size_t	len;
	t_var	*user;
	t_var	*pwd;

	len = 0;
	user = get_var(shell->env, "USER");
	pwd = get_var(shell->env, "PWD");
	write(STDOUT_FILENO, "\033[33;1m", 7);
	len += write(STDOUT_FILENO, user == NULL ? "guest" : user->value
			, user == NULL ? 5 : ft_strlen(user->value));
	write(STDOUT_FILENO, "\033[0m", 4);
	len += write(STDOUT_FILENO, " ", 1);
	write(STDOUT_FILENO, "\033[34;1m", 7);
	len += write(STDOUT_FILENO, pwd == NULL ? "unknown" : pwd->value
			, pwd == NULL ? 7 : ft_strlen(pwd->value));
	write(STDOUT_FILENO, "\033[0m", 4);
	len += write(STDOUT_FILENO, " > ", 3);
	return (len);
}

static size_t	print_prompt_with_type(t_shell *shell)
{
	char	*str;

	str = "";
	if (shell->term.prompt == PROMPT_ESCAPED)
		str = "> ";
	else if (shell->term.prompt == PROMPT_QUOTE)
		str = "quote> ";
	else if (shell->term.prompt == PROMPT_DQUOTE)
		str = "dquote> ";
	else if (shell->term.prompt == PROMPT_HEREDOC)
		str = "heredoc> ";
	else if (shell->term.prompt == PROMPT_BRACKET)
		str = "bracket> ";
	else if (shell->term.prompt == PROMPT_OPE)
		str = "pipe> ";
	return (write(STDOUT_FILENO, str, ft_strlen(str)));
}

void			show_prompt(t_shell *shell)
{
	if (shell->term.visual_mode)
		write(STDOUT_FILENO, "(visual) ", 9);
	shell->term.offset = (shell->term.prompt == PROMPT_DEF)
		? print_def_prompt(shell) : print_prompt_with_type(shell);
}
