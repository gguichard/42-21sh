/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 15:40:38 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/23 13:51:19 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"
#include "utils.h"

int		handle_new_line(t_shell *shell, t_term *term)
{
	if (term->visual_mode)
		return (0);
	insert_cmdline(shell, term, '\n');
	return (1);
}

int		handle_screen_clear(t_shell *shell, t_term *term)
{
	tputs(tgetstr("cl", NULL), 1, t_putchar);
	go_to_prompt(term);
	print_input(shell, term);
	return (1);
}

int		handle_bell(t_shell *shell, t_term *term)
{
	(void)shell;
	(void)term;
	tputs(tgetstr("bl", NULL), 1, t_putchar);
	return (1);
}
