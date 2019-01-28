/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/10 16:28:43 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/28 15:12:31 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	count_same_char(const char *str1, const char *str2)
{
	int	count;

	count = 0;
	while (str1[count] != '\0' && str1[count] == str2[count])
		++count;
	return (count);
}

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
