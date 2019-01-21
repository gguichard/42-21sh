/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 12:09:56 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/21 12:12:37 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "shell.h"
#include "execute.h"
#include "libft.h"

char	**arg_lst_to_tab(t_list *arg_lst)
{
	size_t	size;
	t_list	*curr;
	char	**args;
	size_t	index;

	size = 0;
	curr = arg_lst;
	while (curr != NULL)
	{
		size++;
		curr = curr->next;
	}
	if ((args = (char **)malloc((size + 1) * sizeof(char *))) != NULL)
	{
		curr = arg_lst;
		index = -1;
		while (++index < size && curr != NULL)
		{
			if ((args[index] = (char *)curr->content) == NULL)
				return (ft_strtab_free(args));
			curr = curr->next;
		}
		args[index] = NULL;
	}
	return (args);
}
