/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 16:28:43 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/10 16:41:37 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	strlist_insert_sort(t_list **lst, t_list *elem)
{
	t_list	*prev;
	t_list	*curr;

	prev = NULL;
	curr = *lst;
	while (curr != NULL)
	{
		if (ft_strcmp((char *)elem->content, (char *)curr->content) <= 0)
			break ;
		prev = curr;
		curr = curr->next;
	}
	if (prev == NULL)
	{
		elem->next = *lst;
		*lst = elem;
	}
	else
	{
		elem->next = curr;
		prev->next = elem;
	}
}
