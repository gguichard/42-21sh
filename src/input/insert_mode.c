/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_mode.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/08 14:28:03 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/11 00:49:55 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "shell.h"
#include "input.h"
#include "vars.h"
#include "autocomplete.h"
#include "utils.h"

void		init_ac_format(t_ac_format *fmt, t_list *lst, t_term *term)
{
	fmt->elems = 0;
	fmt->col_width = 0;
	while (lst != NULL)
	{
		(fmt->elems)++;
		fmt->col_width = ft_max(ft_strlen((char *)lst->content)
				, fmt->col_width);
		lst = lst->next;
	}
	fmt->col_width += 2;
	fmt->max_col = term->winsize.ws_col / fmt->col_width;
	fmt->max_row = (int)ft_ceil(fmt->elems / (double)fmt->max_col);
	fmt->max_col = (int)ft_ceil(fmt->elems / (double)fmt->max_row);
}

t_ac_format	*ac_get_format(t_list *lst, t_term *term)
{
	t_ac_format	*fmt;
	int			col;
	int			row;

	if ((fmt = (t_ac_format *)malloc(sizeof(t_ac_format))) == NULL)
		return (NULL);
	init_ac_format(fmt, lst, term);
	if (fmt->max_col <= 0
			|| !(fmt->cols = (t_list **)malloc(fmt->max_col* sizeof(t_list *))))
	{
		free(fmt);
		return (NULL);
	}
	(fmt->cols)[0] = lst;
	col = 1;
	while (col < fmt->max_col)
	{
		row = 0;
		while (lst != NULL && row < fmt->max_row)
		{
			lst = lst->next;
			row++;
		}
		(fmt->cols)[col] = lst;
		col++;
	}
	return (fmt);
}

void		ac_print_list(t_list *lst, t_term *term)
{
	t_ac_format	*fmt;
	int			row;
	int			col;

	if ((fmt = ac_get_format(lst, term)) == NULL)
		return ;
	ft_putchar('\n');
	row = 0;
	while (row < fmt->max_row)
	{
		col = 0;
		while (col < fmt->max_col && (fmt->cols)[col] != NULL)
		{
			if (col + 1 == fmt->max_col)
				ft_printf("%s", (fmt->cols)[col]->content);
			else
				ft_printf("%-*s", fmt->col_width, (fmt->cols)[col]->content);
			(fmt->cols)[col] = (fmt->cols)[col]->next;
			col++;
		}
		ft_putchar('\n');
		row++;
	}
	free(fmt->cols);
	free(fmt);
}

int			handle_ac(t_shell *shell, t_term *term)
{
	t_var				*path;
	size_t				cursor;
	t_ac_suff_inf		*result;
	char				*curr;

	if ((path = get_var(shell->env, "PATH")) == NULL)
		return (0);
	cursor = term->cursor;
	while (cursor > 0 && term->line[cursor - 1] != ' ')
		cursor--;
	result = autocomplet_word(&(term->line[cursor]), 1, path->value
			, &(shell->builtins));
	if (result == NULL || result->suff == NULL)
	{
		delete_ac_suff_inf(result);
		return (0);
	}
	curr = result->suff;
	while (*curr != '\0')
	{
		insert_cmdline(shell, term, *curr);
		curr++;
	}
	if (term->ac_flag)
	{
		ac_print_list(result->choices, term);
		refresh_prompt_command(shell, term);
	}
	(term->ac_flag)++;
	delete_ac_suff_inf(result);
	return (1);
}

int			handle_eot_key(t_shell *shell, t_term *term)
{
	(void)shell;
	if (term->size == 0)
		return (1);
	tputs(tgetstr("bl", NULL), 1, t_putchar);
	return (0);
}

int			handle_key(t_shell *shell, t_term *term, char key)
{
	int	ret;
	int	ac;

	ret = 1;
	ac = term->ac_flag;
	if (key >= 32 && key < 127)
		insert_cmdline(shell, term, key);
	else if (key == EOT_KEY)
		return (handle_eot_key(shell, term) ? -1 : 1);
	else if (key == BACKSPACE_KEY)
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
