/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 21:25:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/11 12:47:48 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "get_next_line.h"
#include "shell.h"
#include "input.h"
#include "history.h"

//TODO SUPPRIMER
#include "check_enclosing_char_cmd.h"
#include "split_cmd_semicolon.h"
#include "str_cmd_inf.h"
//FIN

int	handle_command(t_shell *shell)
{
	ft_printf("COMMAND: %s\n", shell->term.line);
	//TODO SUPPRIMER
	{
		t_str_cmd_inf str_cmd_inf;
		scmd_init(&str_cmd_inf, shell->term.line);
		t_str_cmd_err str_cmd_err = check_enclosing_char_cmd(&str_cmd_inf);
		ft_printf("ENCLOSING CHAR ERROR: ");
		if (str_cmd_err == SCMDERR_ENDTOOSOON)
		{
			ft_printf("END_TO_SOON\n");
		}
		else if (str_cmd_err == SCMDERR_BADCHAR)
		{
			ft_printf("BAD_CHAR\n");
		}
		else if (str_cmd_err == SCMDERR_NOERROR)
		{
			ft_printf("NOTHING\n");
			t_list *all_sub_cmd = split_cmd_semicolon(&str_cmd_inf);
			t_list *cur_sub_cmd = all_sub_cmd;
			while (cur_sub_cmd != NULL)
			{
				ft_printf("SUB_COMMAND: %s\n", (char*)cur_sub_cmd->content);
				cur_sub_cmd = cur_sub_cmd->next;
			}
			ft_lstfree(&all_sub_cmd);
		}
		else
		{
			ft_printf("UNKNOWN_ERROR\n");
		}
	}
	//FIN
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
