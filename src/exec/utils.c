/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 12:09:56 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/29 17:58:32 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "check_path.h"
#include "execute.h"

static char	*get_bin_path(t_shell *shell, char *value, int is_from_hashtable
		, t_error *error)
{
	char	*bin_path;

	bin_path = NULL;
	if (!is_from_hashtable && !ft_strchr(value, '/'))
		bin_path = search_binary(shell, value, error);
	else
	{
		if ((bin_path = ft_strdup(value)) == NULL)
		{
			*error = ERRC_UNEXPECTED;
			return (NULL);
		}
		*error = check_file_rights(value, FT_FILE, X_OK);
	}
	return (bin_path);
}

char		*get_cmd_inf_path(t_shell *shell, t_cmd_inf *cmd_inf
		, t_error *error)
{
	char		*name;
	t_hashentry	*hashentry;
	char		*bin_path;

	name = (char *)(cmd_inf->arg_lst->content);
	hashentry = NULL;
	if (shell->exec_hashtable != NULL)
	{
		hashentry = get_hashentry(shell->exec_hashtable, name);
		if (hashentry != NULL)
			name = hashentry->value;
	}
	*error = ERRC_NOERROR;
	bin_path = get_bin_path(shell, name, hashentry != NULL, error);
	if (bin_path != NULL && shell->exec_hashtable != NULL && hashentry == NULL)
		add_hashentry(shell->exec_hashtable, name, bin_path
				, ft_strlen(bin_path) + 1);
	return (bin_path);
}

char		**arg_lst_to_tab(t_list *arg_lst)
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
