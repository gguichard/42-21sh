/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 09:54:41 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/09 12:44:37 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "shell.h"

void		clear_history_entry(t_history *elem)
{
	free(elem->content);
	free(elem);
}

void		clear_history(t_shell *shell)
{
	t_history	*curr;
	t_history	*next;

	if (shell->history != NULL)
	{
		curr = shell->history->next;
		while (curr != NULL)
		{
			next = curr->next;
			clear_history_entry(curr);
			curr = next;
		}
		curr = shell->history->prev;
		while (curr != NULL)
		{
			next = curr->prev;
			clear_history_entry(curr);
			curr = next;
		}
		clear_history_entry(shell->history);
		shell->history = NULL;
	}
}

void		add_history_entry(t_shell *shell, const char *command)
{
	t_history	*elem;
	t_history	*curr;

	elem = (t_history *)malloc(sizeof(t_history));
	if (elem == NULL || (elem->content = ft_strdup(command)) == NULL)
	{
		free(elem);
		return ;
	}
	curr = shell->history;
	while (curr != NULL && curr->next != NULL)
		curr = curr->next;
	elem->next = curr;
	if (curr == NULL)
	{
		elem->prev = NULL;
		shell->history = elem;
	}
	else
	{
		if ((elem->prev = curr->prev) != NULL)
			elem->prev->next = elem;
		curr->prev = elem;
		shell->history = curr;
	}
}

const char	*peek_history_prev(t_shell *shell)
{
	t_history	*elem;

	elem = shell->history;
	if (elem == NULL || elem->prev == NULL)
		return (NULL);
	shell->history = elem->prev;
	return (elem->prev->content);
}

const char	*peek_history_next(t_shell *shell)
{
	t_history	*elem;

	elem = shell->history;
	if (elem == NULL || elem->next == NULL)
		return (NULL);
	shell->history = elem->next;
	return (elem->next->content);
}
