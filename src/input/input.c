/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 21:25:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/27 17:32:32 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "shell.h"
#include "input.h"
#include "error.h"
#include "history.h"
#include "expand_vars.h"
#include "apply_escape.h"
#include "lexer.h"
#include "execute.h"
#include "split_cmd_token.h"

char		*get_command_line(t_term *term)
{
	size_t	len;
	char	*line;
	int		add_nl;

	if (term->prev_lines == NULL)
		return (ft_strdup(term->line));
	add_nl = term->prompt != PROMPT_OPE;
	len = ft_strlen(term->prev_lines);
	line = (char *)malloc((len + term->size + add_nl + 1) * sizeof(char));
	if (line != NULL)
	{
		ft_memcpy(line, term->prev_lines, len);
		ft_memcpy(line + len + add_nl, term->line, term->size);
		line[len + add_nl + term->size] = '\0';
		if (add_nl)
			line[len] = '\n';
	}
	return (line);
}

static char	*handle_input(t_shell *shell, t_error *error)
{
	int				ret;
	char			*input;
	t_str_cmd_inf	scmd_inf;
	t_list			*token_lst;

	input = NULL;
	*error = ERRC_NOERROR;
	while ((ret = read_input(shell)) == 1)
	{
		input = get_command_line(&(shell->term));
		ft_strdel(&(shell->term.prev_lines));
		if (input == NULL)
		{
			*error = ERRC_UNEXPECTED;
			break ;
		}
		scmd_init(&scmd_inf, input);
		if ((token_lst = split_cmd_token(&scmd_inf)) != NULL)
		{
			if (!lex_commands(token_lst))
				*error = ERRC_LEXERROR;
			else if (is_command_complete(&scmd_inf, token_lst))
				*error = ERRC_NOERROR;
			else
			{
				*error = ERRC_INCOMPLETECMD;
				shell->term.prev_lines = input;
			}
			ft_lstdel(&token_lst, del_token);
		}
		scmd_delete(scmd_inf.sub_var_bracket);
		if (*error != ERRC_INCOMPLETECMD)
			break ;
		reset_cmdline(shell, get_prompt_type(&scmd_inf));
	}
	shell->term.prev_lines = NULL;
	if (ret < 0)
	{
		*error = ERRC_UNEXPECTED;
		ft_strdel(&input);
	}
	return (input);
}

int			handle_command(t_shell *shell)
{
	t_error			error;
	char			*input;
	char			*line;
	char			*expand_line;
	char			*var_error;
	t_str_cmd_inf	scmd_inf;
	t_list			*token_lst;

	error = ERRC_NOERROR;
	if ((input = handle_input(shell, &error)) == NULL)
		return (-1);
	line = apply_only_newline_escape(input, 0);
	free(input);
	if (line == NULL)
		return (-1);
	if (error == ERRC_INCOMPLETECMD)
		ft_dprintf(2, "%s: unexpected end of file\n", ERR_PREFIX);
	else if (error == ERRC_NOERROR)
	{
		var_error = NULL;
		expand_line = expand_vars(line, shell, &var_error);
		if (expand_line == NULL && var_error == NULL)
		{
			free(line);
			return (0);
		}
		if (var_error != NULL)
		{
			ft_dprintf(2, "%s: %s: Bad substitution\n", ERR_PREFIX, var_error);
			free(var_error);
		}
		else
		{
			scmd_init(&scmd_inf, expand_line);
			token_lst = split_cmd_token(&scmd_inf);
			if (token_lst != NULL)
			{
				execute_all(shell, token_lst);
				ft_lstdel(&token_lst, del_token);
			}
			scmd_delete(scmd_inf.sub_var_bracket);
			free(expand_line);
		}
	}
	add_history_entry(shell, line);
	free(line);
	return (1);
}

int			wait_for_command(t_shell *shell)
{
	int	ret;

	if (!realloc_cmdline(&(shell->term)))
		return (0);
	ret = 1;
	while (ret > 0)
	{
		reset_cmdline(shell, PROMPT_DEF);
		ret = handle_command(shell);
	}
	ft_putendl("exit");
	return (ret);
}
