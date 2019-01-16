/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 10:52:35 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/16 02:09:17 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"

size_t	get_total_rows(t_term *term)
{
	size_t	rows;
	size_t	index;
	size_t	col;

	rows = 1;
	index = 0;
	col = term->offset;
	while (index < term->size)
	{
		if ((col + 1) % term->winsize.ws_col != 0
				&& (term->line)[index] != '\n')
			col++;
		else
		{
			col = 0;
			rows++;
		}
		index++;
	}
	return (rows);
}

void	update_cursor_data(t_term *term)
{
	term->rows = get_total_rows(term);
}

size_t	get_max_col(t_term *term)
{
	return (term->winsize.ws_col - 1);
}
