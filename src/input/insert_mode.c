/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_mode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 14:28:03 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/28 10:45:04 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "str_cmd_inf.h"
#include "shell.h"
#include "input.h"
#include "apply_escape.h"
#include "vars.h"
#include "utils.h"

static void		add_char_and_escape_if_needed(char char_to_add
		, int already_escaped, t_shell *shell, t_str_cmd_inf *scmd)
{
	if (char_to_add == '\0')
		return ;
	else if (!already_escaped)
	{
		if ((scmd_cur_char_is_in_nothing(scmd)
					&& ft_strchr(" \t|<>;$\'\"\\", char_to_add) != NULL)
				|| (scmd->is_in_doublequote
					&& ft_strchr("$\"\\", char_to_add) != NULL))
		{
			insert_cmdline(shell, &(shell->term), '\\');
			insert_cmdline(shell, &(shell->term), char_to_add);
		}
		else if (scmd_cur_char_is_in_nothing(scmd) && char_to_add == '\n')
		{
			insert_cmdline(shell, &(shell->term), '\'');
			insert_cmdline(shell, &(shell->term), char_to_add);
			insert_cmdline(shell, &(shell->term), '\'');
		}
		else if (scmd->is_in_quote && char_to_add == '\'')
		{
			insert_cmdline(shell, &(shell->term), '\'');
			insert_cmdline(shell, &(shell->term), '\\');
			insert_cmdline(shell, &(shell->term), char_to_add);
			insert_cmdline(shell, &(shell->term), '\'');
		}
		else
			insert_cmdline(shell, &(shell->term), char_to_add);
	}
	else if (char_to_add == '\n')
	{
		handle_bs_key(shell, &(shell->term));
		add_char_and_escape_if_needed(char_to_add, 0, shell, scmd);
	}
	else
		insert_cmdline(shell, &(shell->term), char_to_add);
}

void	ac_append(t_shell *shell, t_term *term, t_ac_suff_inf *result
		, t_str_cmd_inf *scmd)
{
	char	*curr;
	int		is_at_end_of_cmd;

	is_at_end_of_cmd = (scmd->str[scmd->pos] == '\0');
	curr = result->suff;
	while (*curr != '\0')
	{
		add_char_and_escape_if_needed(*curr
				, (curr == result->suff && scmd_cur_char_is_escaped(scmd))
				, shell, scmd);
		++curr;
	}
	if (result->suff_type == ACS_TYPE_FILE && is_at_end_of_cmd)
	{
		if (scmd->is_in_quote)
			insert_cmdline(shell, term, '\'');
		else if (scmd->is_in_doublequote)
			insert_cmdline(shell, term, '\"');
		insert_cmdline(shell, term, ' ');
	}
	else if (result->suff_type == ACS_TYPE_DIR)
		insert_cmdline(shell, term, '/');
}

int		handle_ac(t_shell *shell, t_term *term)
{
	t_ac_suff_inf	*result;
	t_str_cmd_inf	scmd;
	char			*line;
	char			*real_line;

	if ((line = get_command_line(term)) == NULL)
		return (0);
	line[ft_strlen(line) - (term->size - term->cursor)] = '\0';
	if ((real_line = apply_only_newline_escape(line)) == NULL)
	{
		free(line);
		return (0);
	}
	free(line);
	scmd_init(&scmd, real_line);
	result = autocomplete_cmdline(&scmd, shell
			, &(shell->builtins));
	if (result == NULL || result->suff == NULL || result->choices == NULL)
	{
		scmd_delete(scmd.sub_var_bracket);
		free(real_line);
		return (!!delete_ac_suff_inf(result));
	}
	ac_append(shell, term, result, &scmd);
	if ((term->ac_flag)++)
	{
		tputs(tparm(tgetstr("do", NULL), term->rows - term->row), 1, t_putchar);
		tputs(tgetstr("cr", NULL), 1, t_putchar);
		ac_print_list(result->choices, term);
		print_input(shell, term);
	}
	scmd_delete(scmd.sub_var_bracket);
	delete_ac_suff_inf(result);
	free(real_line);
	return (1);
}

int		handle_eot_key(t_shell *shell, t_term *term)
{
	(void)shell;
	if (term->size == 0)
		return (1);
	tputs(tgetstr("bl", NULL), 1, t_putchar);
	return (0);
}

int		handle_key(t_shell *shell, t_term *term, char key)
{
	int	ret;
	int	ac;

	ret = 1;
	ac = term->ac_flag;
	if (key >= 32 && key < 127)
		insert_cmdline(shell, term, key);
	else if (key == EOT_KEY)
		return (handle_eot_key(shell, term) ? -1 : 1);
	else if (key == BACKSPACE_KEY || key == DELETE_KEY)
		ret = handle_bs_key(shell, term);
	else if (key == '\t')
		ret = handle_ac(shell, term);
	else if (key == '\n')
	{
		move_cursor_end(shell, term);
		ft_putchar('\n');
		return (0);
	}
	if (!ret)
		tputs(tgetstr("bl", NULL), 1, t_putchar);
	if (ac && ac == term->ac_flag)
		term->ac_flag = 0;
	return (1);
}
