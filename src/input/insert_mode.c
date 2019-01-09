/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_mode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 14:28:03 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/09 12:39:57 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "shell.h"
#include "input.h"
#include "vars.h"
#include "autocomplete.h"
#include "utils.h"

int	try_autocomplete(t_shell *shell, t_term *term)
{
	t_var				*path;
	size_t				cursor;
	char				*result;
	char				*curr;

	if ((path = get_var(shell->env, "PATH")) == NULL)
		return (0);
	cursor = term->cursor;
	while (cursor > 0 && term->line[cursor - 1] != ' ')
		cursor--;
	result = autocomplet_word(&(term->line[cursor]), 1, path->value
			, &(shell->builtins));
	if (result == NULL || result[0] == '\0')
	{
		free(result);
		return (0);
	}
	curr = result;
	while (*curr != '\0')
	{
		insert_cmdline(shell, term, *curr);
		curr++;
	}
	free(result);
	return (1);
}

int	handle_key(t_shell *shell, t_term *term, char key)
{
	if (key == '\n')
	{
		ft_putchar('\n');
		return (0);
	}
	else if (key == '\t')
	{
		if (!try_autocomplete(shell, term))
			tputs(tgetstr("bl", NULL), 1, t_putchar);
		return (1);
	}
	else if (key == BACKSPACE_KEY)
	{
		handle_bs_key(shell, term);
		return (1);
	}
	insert_cmdline(shell, term, key);
	return (1);
}
