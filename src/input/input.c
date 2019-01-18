/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 21:25:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/18 15:00:28 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "get_next_line.h"
#include "shell.h"
#include "input.h"
#include "history.h"
#include "lexer.h"

//TODO SUPPRIMER
#include "split_cmd_token.h"
#include "str_cmd_inf.h"
#include "cmd_inf.h"
#include "join_token_cmd.h"
#include "apply_escape.h"
#include "token_inf.h"
//FIN

//TODO SUPPRIMER
void	print_cmd(t_cmd_inf *cmd_inf, int padding)
{
	t_list	*args;
	char	*tmp;

	ft_printf("%*s|", padding, "");
	args = cmd_inf->arg_lst;
	while (args != NULL)
	{
		tmp = apply_escape((char*)(args->content));
		ft_printf(" \"%s\"", tmp);
		free(tmp);
		args = args->next;
	}
	ft_printf("\n");
	if (cmd_inf->pipe_cmd != NULL)
	{
		print_cmd(cmd_inf->pipe_cmd, padding + 4);
	}
}

void	debug_tokens(t_list *all_sub_cmd)
{
	t_list	*cmd_lst;

	cmd_lst = NULL;
	if (all_sub_cmd == NULL)
		ft_printf("   TOKEN NULL\n");
	else
		cmd_lst = join_token_cmd(all_sub_cmd);
	while (all_sub_cmd != NULL)
	{
		ft_printf("TOKEN:\n");
		if (((t_token_inf*)all_sub_cmd->content)->type == TK_WORD)
		{
			ft_printf("   =WORD\n");
		}
		else if (((t_token_inf*)all_sub_cmd->content)->type == TK_NUM_OPT)
		{
			ft_printf("   =NUM_OPT\n");
		}
		else if (((t_token_inf*)all_sub_cmd->content)->type == TK_OPE)
		{
			ft_printf("   =OPE\n");
		}
		else if (((t_token_inf*)all_sub_cmd->content)->type == TK_CMD_SEP)
		{
			ft_printf("   =CMD_SEP\n");
		}
		else if (((t_token_inf*)all_sub_cmd->content)->type == TK_STR_OPT)
		{
			ft_printf("   =STR_OPT\n");
		}
		else if (((t_token_inf*)all_sub_cmd->content)->type == TK_NOTHING)
		{
			ft_printf("   =NOTHING (error)\n");
		}
		else
		{
			ft_printf("   =ERROR\n");
		}
		ft_printf("      =%s=\n", ((t_token_inf*)all_sub_cmd->content)->token);
		all_sub_cmd = all_sub_cmd->next;
	}
	ft_printf("PARSED COMMAND:\n");
	while (cmd_lst != NULL)
	{
		print_cmd((t_cmd_inf*)(cmd_lst->content), 0);
		cmd_lst = cmd_lst->next;
	}
}
//FIN

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
		if (parse_commands(all_sub_cmd)
				&& (!scmd_cur_char_is_in_nothing(&str_cmd_inf)
					|| scmd_cur_char_is_escaped(&str_cmd_inf)))
			shell->term.multiline = line;
		else
		{
			ft_printf("COMMAND: %s\n", line);
			debug_tokens(all_sub_cmd);
		}
		ft_lstdel(&all_sub_cmd, del_token);
	}
	add_history_entry(shell, line);
	if (line != shell->term.multiline)
		ft_strdel(&line);
	return (1);
}

int		handle_key_mode(t_shell *shell, t_term *term, char key)
{
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
			return (handle < 0 ? handle : ret);
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
			ret = get_next_line(STDIN_FILENO, &(shell->term.line));
		if (ret > 0)
			handle_command(shell);
	}
	ft_putendl("exit");
	return (ret);
}
