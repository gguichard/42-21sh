/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 21:25:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/12 21:19:04 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "get_next_line.h"
#include "shell.h"
#include "input.h"
#include "history.h"
#include "parser.h"

//TODO SUPPRIMER
#include "split_cmd_token.h"
#include "str_cmd_inf.h"
#include "token_inf.h"
//FIN

//TODO SUPPRIMER
void	debug_tokens(t_list *all_sub_cmd)
{
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
		else
		{
			ft_printf("   =ERROR\n");
		}
		ft_printf("      =%s=\n", ((t_token_inf*)all_sub_cmd->content)->token);
		all_sub_cmd = all_sub_cmd->next;
	}
}
//FIN

int	handle_command(t_shell *shell)
{
	t_str_cmd_inf	str_cmd_inf;
	t_list 			*all_sub_cmd; 

	ft_printf("COMMAND: %s\n", shell->term.line);
	scmd_init(&str_cmd_inf, shell->term.line);
	all_sub_cmd = split_cmd_token(&str_cmd_inf);
	debug_tokens(all_sub_cmd);
	split_commands(all_sub_cmd);
	ft_lstfree(&all_sub_cmd);
	if (shell->term.size > 0)
		add_history_entry(shell, shell->term.line);
	ft_strdel(&(shell->term.def_line));
	return (1);
}

int	handle_key_mode(t_shell *shell, t_term *term, char key)
{
	if (term->visual_mode)
		return (handle_vm_key(shell, term, key));
	else
		return (handle_key(shell, term, key));
}

int	read_input(t_shell *shell)
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

int	wait_for_command(t_shell *shell)
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
