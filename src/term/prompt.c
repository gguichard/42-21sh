/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 17:10:37 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/29 18:27:23 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "shell.h"
#include "vars.h"

static size_t	write_pwd_with_home_replaced(t_shell *shell)
{
	t_var	*pwd;
	char	*home;
	char	*tmp;
	size_t	len;

	pwd = get_var(shell->env, "PWD");
	if (pwd == NULL)
		return (write(STDOUT_FILENO, "unknown", 7));
	home = get_shell_var(shell, "HOME");
	tmp = NULL;
	if (home != NULL)
	{
		tmp = ft_strstr(pwd->value, home);
		if (tmp == NULL || tmp != pwd->value)
		{
			free(home);
			return (write(STDOUT_FILENO, pwd->value, ft_strlen(pwd->value)));
		}
		tmp += ft_strlen(home);
		free(home);
	}
	len = 0;
	len += write(STDOUT_FILENO, "~", 1);
	if (*tmp != '\0')
		len += write(STDOUT_FILENO, tmp, ft_strlen(tmp));
	return (len);
}

static size_t	print_def_prompt(t_shell *shell)
{
	size_t	len;
	t_var	*user;

	len = 0;
	user = get_var(shell->env, "USER");
	write(STDOUT_FILENO, "\033[33;1m", 7);
	len += write(STDOUT_FILENO, user == NULL ? "guest" : user->value
			, user == NULL ? 5 : ft_strlen(user->value));
	write(STDOUT_FILENO, "\033[0m", 4);
	len += write(STDOUT_FILENO, " ", 1);
	write(STDOUT_FILENO, "\033[34;1m", 7);
	len += write_pwd_with_home_replaced(shell);
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
	size_t	write_len;

	if (shell->term.visual_mode)
		write(STDOUT_FILENO, "(visual) ", 9);
	write_len = (shell->term.prompt == PROMPT_DEF) ? print_def_prompt(shell)
		: print_prompt_with_type(shell);
	shell->term.offset = write_len;
}
