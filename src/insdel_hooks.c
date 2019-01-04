/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insdel_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 16:06:26 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/04 17:21:30 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "utils.h"

void	handle_bs_key(t_term *term)
{
	if (term->cursor <= 0)
	{
		tputs(tgetstr("bl", NULL), 1, term_putchar);
		return ;
	}
	ft_memcpy(&(term->cmdline[term->cursor - 1])
			, &(term->cmdline[term->cursor])
			, term->cmdline_siz - term->cursor + 1);
	(term->cursor)--;
	(term->cmdline_siz)--;
	tputs(tgetstr("le", NULL), 1, term_putchar);
	tputs(tgetstr("dc", NULL), 1, term_putchar);
}

void	handle_del_key(t_term *term)
{
	if (term->cursor >= term->cmdline_siz)
	{
		tputs(tgetstr("bl", NULL), 1, term_putchar);
		return ;
	}
	ft_memmove(&(term->cmdline[term->cursor])
			, &(term->cmdline[term->cursor + 1])
			, term->cmdline_siz - term->cursor + 1);
	(term->cmdline_siz)--;
	tputs(tgetstr("dc", NULL), 1, term_putchar);
}
