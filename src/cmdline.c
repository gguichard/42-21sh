/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 10:05:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/04 10:45:49 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "input.h"

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
	elem->prev = shell->term.cmdline;
	elem->next = NULL;
	if (elem->prev != NULL)
		elem->prev->next = elem;
	shell->term.cmdline = elem;
	(shell->term.cmdline_size)++; 
}
