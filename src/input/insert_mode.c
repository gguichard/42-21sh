/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_mode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 14:28:03 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/22 16:39:10 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"
#include "vars.h"
#include "utils.h"

void	ac_append(t_shell *shell, t_term *term, t_ac_suff_inf *result
		, char ending_char)
{
	char	*curr;

	curr = result->suff;
	while (*curr != '\0')
	{
		if (*curr == ' ')
			insert_cmdline(shell, term, '\\');
		insert_cmdline(shell, term, *curr);
		curr++;
	}
	if (result->suff_type == ACS_TYPE_FILE)
	{
		if (ending_char != '\0')
			insert_cmdline(shell, term, ending_char);
		insert_cmdline(shell, term, ' ');
	}
	else if (result->suff_type == ACS_TYPE_DIR)
		insert_cmdline(shell, term, '/');
}

int		handle_ac(t_shell *shell, t_term *term)
{
	t_var			*path;
	t_ac_suff_inf	*result;
	char			ending_char;

	if ((path = get_var(shell->env, "PATH")) == NULL)
		return (0);
	result = autocomplete_cmdline(term->line, path->value
			, &(shell->builtins), &ending_char);
	if (result == NULL || result->suff == NULL || result->choices == NULL)
	{
		delete_ac_suff_inf(result);
		return (0);
	}
	ac_append(shell, term, result, ending_char);
	if ((term->ac_flag)++)
	{
		tputs(tparm(tgetstr("do", NULL), term->rows - term->row), 1, t_putchar);
		tputs(tgetstr("cr", NULL), 1, t_putchar);
		ac_print_list(result->choices, term);
		refresh_cmdline(shell, term);
	}
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
		ft_putchar('\n');
		return (0);
	}
	if (!ret)
		tputs(tgetstr("bl", NULL), 1, t_putchar);
	if (ac && ac == term->ac_flag)
		term->ac_flag = 0;
	return (1);
}
