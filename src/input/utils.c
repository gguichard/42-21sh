/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 10:52:35 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/15 10:58:20 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"

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
	if (!ft_strchr(term->line, '\n'))
	{
		if ((term->row + 1) < term->rows)
			return (term->winsize.ws_col - 1);
		return ((term->size + term->offset) % term->winsize.ws_col);
	}
	return (0);
}
