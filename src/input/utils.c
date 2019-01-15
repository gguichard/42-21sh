/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 10:52:35 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/15 16:26:48 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"

size_t	get_cursor_col(t_term *term)
{
	size_t	index;
	size_t	row;
	size_t	col;

	index = 0;
	row = 0;
	col = term->offset;
	while (index < term->cursor)
	{
		if ((term->line)[index] != '\n'
				&& (col + 1) % term->winsize.ws_col != 0)
			col++;
		else
		{
			col = 0;
			row++;
		}
		index++;
	}
	return (col);
}

void	update_pos(t_term *term)
{
	size_t	index;
	size_t	row;
	size_t	col;

	index = 0;
	row = 0;
	col = term->offset;
	while (index < term->cursor)
	{
		if ((term->line)[index] != '\n'
				&& (col + 1) % term->winsize.ws_col != 0)
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
	term->rows = get_rows(term);
}

size_t	get_rows(t_term *term)
{
	size_t	index;
	size_t	rows;
	size_t	cursor;

	index = 0;
	rows = 1;
	cursor = term->offset;
	while (index < term->size)
	{
		if ((term->line)[index] != '\n'
				&& (cursor + 1) % term->winsize.ws_col != 0)
			cursor++;
		else
		{
			cursor = 0;
			rows++;
		}
		index++;
	}
	return (rows);
}

size_t	get_max_col(t_term *term)
{
	size_t	index;
	size_t	row;
	size_t	col;

	index = 0;
	row = 0;
	col = term->offset;
	while (row < term->row)
	{
		if ((term->line)[index] != '\n'
				&& (col + 1) % term->winsize.ws_col != 0)
			col++;
		else
		{
			col = 0;
			row++;
		}
		index++;
	}
	col = 0;
	while (col < (term->winsize.ws_col - 1))
	{
		if ((term->line)[index + col] == '\0'
				|| (term->line)[index + col] == '\n')
			break ;
		col++;
	}
	return (col + (term->row == 0 ? term->offset : 0));
}
