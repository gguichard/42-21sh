/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 21:25:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/23 16:24:36 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "get_next_line.h"
#include "shell.h"
#include "input.h"
#include "history.h"
#include "lexer.h"
#include "execute.h"
#include "split_cmd_token.h"

char	*get_command_line(t_term *term)
{
	size_t	ml_len;
	char	*line;

	if (term->multiline == NULL)
		return (ft_strdup(term->line));
	ml_len = ft_strlen(term->multiline);
	line = (char *)malloc((ml_len + term->size + 2) * sizeof(char));
	if (line != NULL)
	{
		ft_memcpy(line, term->multiline, ml_len);
		ft_memcpy(line + ml_len + 1, term->line, term->size);
		line[ml_len + 1 + term->size] = '\0';
		line[ml_len] = '\n';
	}
	ft_strdel(&(term->multiline));
	return (line);
}

int		handle_command(t_shell *shell)
{
	char			*line;
	t_str_cmd_inf	str_cmd_inf;
	t_list			*all_sub_cmd;

	ft_strdel(&(shell->term.def_line));
	line = get_command_line(&(shell->term));
	if (line == NULL)
		return (0);
	scmd_init(&str_cmd_inf, line);
	all_sub_cmd = split_cmd_token(&str_cmd_inf);
	if (all_sub_cmd != NULL)
	{
		if (parse_commands(all_sub_cmd))
		{
			if (is_multiline(&(shell->term), &str_cmd_inf))
				shell->term.multiline = line;
			else
				execute_all(shell, all_sub_cmd);
		}
		ft_lstdel(&all_sub_cmd, del_token);
	}
	if (shell->term.size > 0)
		add_history_entry(shell, line);
	if (line != shell->term.multiline)
		ft_strdel(&line);
	scmd_delete(str_cmd_inf.sub_var_bracket);
	return (1);
}

int		handle_key_mode(t_shell *shell, t_term *term, char key)
{
	if (key == FORM_FEED_KEY)
		return (handle_screen_clear(shell, term));
	if (key == BELL_KEY)
		return (handle_bell(shell, term));
	if (term->visual_mode)
		return (handle_vm_key(shell, term, key));
	else
		return (handle_key(shell, term, key));
}

int		read_input(t_shell *shell)
{
	int		ret;
	char	buf;
	int		handle;

	while ((ret = read(STDIN_FILENO, &buf, 1)) > 0)
	{
		if (handle_esc_key(shell, &(shell->term), buf))
		{
			if (!(shell->term.visual_mode))
				shell->term.ac_flag = 0;
			continue ;
		}
		handle = handle_key_mode(shell, &(shell->term), buf);
		if (handle <= 0)
			return (handle < 0 ? 0 : ret);
	}
	return (ret);
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
		if (!(shell->term.legacy_mode))
			ret = read_input(shell);
		else
		{
			ret = get_next_line(STDIN_FILENO, &(shell->term.line));
			if (ret > 0)
				shell->term.size = ft_strlen(shell->term.line);
		}
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
