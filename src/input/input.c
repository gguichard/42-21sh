/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 21:25:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/27 00:47:26 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "get_next_line.h"
#include "shell.h"
#include "input.h"
#include "history.h"
#include "expand_vars.h"
#include "apply_escape.h"
#include "lexer.h"
#include "execute.h"
#include "split_cmd_token.h"

char	*get_command_line(t_term *term)
{
	size_t	ml_len;
	char	*line;
	int		add_nl;

	if (term->multiline == NULL)
		return (ft_strdup(term->line));
	add_nl = term->prompt != PROMPT_OPE;
	ml_len = ft_strlen(term->multiline);
	line = (char *)malloc((ml_len + term->size + add_nl + 1) * sizeof(char));
	if (line != NULL)
	{
		ft_memcpy(line, term->multiline, ml_len);
		ft_memcpy(line + ml_len + add_nl, term->line, term->size);
		line[ml_len + add_nl + term->size] = '\0';
		if (add_nl)
			line[ml_len] = '\n';
	}
	return (line);
}

int		handle_command(t_shell *shell)
{
	char			*tmp_line;
	char			*noexp_line;
	char			*line;
	char			*var_error;
	t_str_cmd_inf	str_cmd_inf;
	t_list			*token_lst;

	ft_strdel(&(shell->term.def_line));
	var_error = NULL;
	if ((tmp_line = get_command_line(&(shell->term))) == NULL)
		return (0);
	ft_strdel(&(shell->term.multiline));
	noexp_line = apply_only_newline_escape(tmp_line, 0);
	free(tmp_line);
	if (noexp_line == NULL || ((line = expand_vars(noexp_line, shell,
						&var_error)) == NULL && var_error == NULL))
	{
		free(noexp_line);
		return (0);
	}
	if (var_error != NULL)
	{
		ft_dprintf(2, "%s: %s: Bad substitution\n", ERR_PREFIX, var_error);
		free(var_error);
	}
	else
	{
		scmd_init(&str_cmd_inf, line);
		token_lst = split_cmd_token(&str_cmd_inf);
		if (token_lst != NULL)
		{
			if (lex_commands(token_lst))
			{
				if (!is_multiline(&str_cmd_inf, token_lst))
					execute_all(shell, token_lst);
				else
				{
					shell->term.prompt = get_prompt_type(&str_cmd_inf);
					shell->term.multiline = line;
				}
			}
			ft_lstdel(&token_lst, del_token);
		}
		scmd_delete(str_cmd_inf.sub_var_bracket);
	}
	if (shell->term.size > 0)
		add_history_entry(shell, noexp_line);
	if (line != NULL && line != shell->term.multiline)
		ft_strdel(&line);
	free(noexp_line);
	return (1);
}

int		wait_for_command(t_shell *shell)
{
	int	ret;

	if (!realloc_cmdline(&(shell->term)))
		return (0);
	ret = 1;
	while (ret > 0)
	{
		reset_cmdline(shell);
		ret = read_input(shell);
		if (ret > 0)
			handle_command(shell);
		else if (shell->term.multiline != NULL)
			ret = handle_multiline_eot(&(shell->term));
		if (shell->term.legacy_mode)
			ft_strdel(&(shell->term.line));
	}
	ft_putendl("exit");
	return (ret);
}
