/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 10:52:35 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/16 14:54:28 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"

size_t	get_total_rows(t_term *term)
{
	size_t	rows;
	size_t	col;
	size_t	index;

	rows = 1;
	col = term->offset;
	index = 0;
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

void	update_pos_data(t_term *term)
{
	size_t	row;
	size_t	col;
	size_t	index;

	row = 0;
	col = term->offset;
	index = 0;
	while (index < term->cursor)
	{
		if ((col + 1) % term->winsize.ws_col != 0
				&& (term->line)[index] != '\n')
			col++;
		else
		{
			col = 0;
			row++;
		}
		index++;
	}
	term->row = row;
	term->col = col;
	term->rows = get_total_rows(term);
}

void	update_cursor_data(t_term *term)
{
	size_t	row;
	size_t	col;
	size_t	index;

	row = 0;
	col = term->offset;
	index = 0;
	while (row < term->row)
	{
		if ((col + 1) % term->winsize.ws_col != 0
				&& (term->line)[index] != '\n')
			col++;
		else
		{
			col = 0;
			row++;
		}
		index++;
	}
	term->cursor = index + (term->col - col);
}

size_t	get_max_col(t_term *term)
{
	size_t	row;
	size_t	col;
	size_t	index;

	row = 0;
	col = term->offset;
	index = 0;
	while (row < term->row)
	{
		if ((col + 1) % term->winsize.ws_col != 0
				&& (term->line)[index] != '\n')
			col++;
		else
		{
			col = 0;
			row++;
		}
		index++;
	}
	while (col < (term->winsize.ws_col - 1))
	{
		if ((term->line)[index] == '\0'
				|| (term->line)[index] == '\n')
			break ;
		col++;
		index++;
	}
	return (col);
}
