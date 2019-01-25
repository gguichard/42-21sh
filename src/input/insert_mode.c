/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_mode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 14:28:03 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/25 09:19:20 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "str_cmd_inf.h"
#include "shell.h"
#include "input.h"
#include "vars.h"
#include "utils.h"

void	ac_append(t_shell *shell, t_term *term, t_ac_suff_inf *result
		, t_str_cmd_inf *scmd)
{
	char	*curr;
	int		is_at_end_of_cmd;

	is_at_end_of_cmd = (scmd->str[scmd->pos] == '\0');
	curr = result->suff;
	while (*curr != '\0')
	{
		if (*curr == ' ' && scmd_cur_char_is_in_nothing(scmd))
			insert_cmdline(shell, term, '\\');
		insert_cmdline(shell, term, *curr);
		curr++;
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
	t_var			*path;
	t_ac_suff_inf	*result;
	t_str_cmd_inf	scmd;
	char			old_char;

	if ((path = get_var(shell->env, "PATH")) == NULL)
		return (0);
	old_char = (term->line)[term->cursor];
	term->line[term->cursor] = '\0';
	scmd_init(&scmd, term->line);
	result = autocomplete_cmdline(&scmd, path->value
			, &(shell->builtins));
	term->line[term->cursor] = old_char;
	if (result == NULL || result->suff == NULL || result->choices == NULL)
	{
		scmd_delete(scmd.sub_var_bracket);
		return (!!delete_ac_suff_inf(result));
	}
	ac_append(shell, term, result, &scmd);
	if ((term->ac_flag)++)
	{
		tputs(tparm(tgetstr("do", NULL), term->rows - term->row), 1, t_putchar);
		tputs(tgetstr("cr", NULL), 1, t_putchar);
		ac_print_list(result->choices, term);
		go_to_prompt(term);
		print_input(shell, term);
	}
	scmd_delete(scmd.sub_var_bracket);
	delete_ac_suff_inf(result);
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
