/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 10:05:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/04 12:21:18 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "input.h"
#include "utils.h"

char	*get_cmdline(t_shell *shell)
{
	t_cmdline	*curr;
	t_cmdline	*prev;

	if (!(shell->term.legacy_mode))
	{
		shell->term.line = (char *)malloc(shell->term.cmdline_size + 1);
		if (shell->term.line != NULL)
		{
			(shell->term.line)[shell->term.cmdline_size] = '\0';
			curr = shell->term.cmdline;
			while (curr != NULL)
			{
				prev = curr->prev;
				(shell->term.line)[--(shell->term.cmdline_size)] = curr->key;
				free(curr);
				curr = prev;
			}
			shell->term.cmdline = NULL;
			shell->term.cmdline_curr = NULL;
		}
	}
	return (shell->term.line);
}

void	append_char_cmdline(t_shell *shell, char key)
{
	t_cmdline	*elem;

	if (!(elem = malloc(sizeof(t_cmdline))))
		return ;
	elem->key = key;
	elem->prev = NULL;
	elem->next = NULL;
	if (shell->term.cmdline_curr == NULL)
	{
		elem->prev = shell->term.cmdline;
		shell->term.cmdline = elem;
	}
	else
	{
		elem->prev = shell->term.cmdline_curr->prev;
		elem->next = shell->term.cmdline_curr;
		shell->term.cmdline_curr = elem->next;
	}
	if (elem->prev != NULL)
		elem->prev->next = elem;
	if (elem->next != NULL)
		elem->next->prev = elem;
	(shell->term.cmdline_size)++;
}

void	handle_esc_key(t_shell *shell, char *seq)
{
	if (ft_strequ(seq, ESC_SEQ_LEFT))
		move_cursor_left(shell);
}
