/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 12:40:55 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/12 16:15:17 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "shell.h"
#include "check_path.h"
#include "token_inf.h"

t_error	check_exec(const char *path, const char *bin)
{
	char	*file_path;
	size_t	path_len;
	size_t	bin_len;
	t_error	ret;

	path_len = ft_strlen(path);
	bin_len = ft_strlen(bin);
	file_path = (char *)malloc(path_len + bin_len + 2);
	if (file_path == NULL)
		return (ERRC_UNEXPECTED);
	ft_memcpy(file_path, path, path_len);
	file_path[path_len] = '/';
	ft_memcpy(&(file_path[path_len + 1]), bin, bin_len + 1);
	ret = check_file_for_exec(file_path);
	free(file_path);
	return (ret);
}

t_list	*execute_command(t_list *tokens)
{
	(void)tokens;
	return (NULL);
}

t_list	*get_commands_list(t_list *tokens)
{
	t_list		*lst;
	t_token_inf	*token_inf;
	int			is_on_streak;

	lst = NULL;
	is_on_streak = 0;
	while (tokens != NULL)
	{
		token_inf = (t_token_inf *)tokens->content;
		if ((is_on_streak && token_inf->type != TK_OPE)
				|| (!is_on_streak && token_inf->type == TK_WORD))
		{
			ft_lstpush(&lst, tokens);
			is_on_streak = !is_on_streak;
		}
		tokens = tokens->next;
	}
	return (lst);
}
