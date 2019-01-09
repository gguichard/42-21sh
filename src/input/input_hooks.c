/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_hooks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 13:55:57 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/09 12:47:12 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "utils.h"

int	handle_eot_key(t_shell *shell, t_term *term)
{
	(void)shell;
	if (term->size == 0)
		return (1);
	tputs(tgetstr("bl", NULL), 1, t_putchar);
	return (0);
}
