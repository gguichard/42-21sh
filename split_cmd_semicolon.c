/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd_semicolon.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 11:20:47 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/09 15:23:58 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "str_cmd_inf.h"
#include "split_cmd_semicolon.h"

t_list	*split_cmd_semicolon(t_str_cmd_inf *str_cmd_inf)
{
	t_list	*cmd_lst;
	t_list	*new_elem;
	char	*after_last_semicolon;
	size_t	cur_cmd_size;

	cmd_lst = NULL;
	after_last_semicolon = str_cmd_inf->str;
	while (str_cmd_inf->str[str_cmd_inf->pos] != '\0')
	{
		if (scmd_cur_char_is_in_nothing(str_cmd_inf)
				&& str_cmd_inf->str[str_cmd_inf->pos] == ';'
				&& !scmd_cur_char_is_escaped(str_cmd_inf))
		{
			cur_cmd_size = (str_cmd_inf->str + str_cmd_inf->pos)
				- after_last_semicolon;
			if ((new_elem = ft_lstnew(after_last_semicolon,
							cur_cmd_size + 1)) == NULL)
				return (ft_lstfree(&cmd_lst));
			((char*)new_elem->content)[cur_cmd_size] = '\0';
			ft_lstpush(&cmd_lst, new_elem);
			after_last_semicolon = (str_cmd_inf->str + str_cmd_inf->pos) + 1;
		}
		scmd_move_to_next_char(str_cmd_inf);
	}
	cur_cmd_size = (str_cmd_inf->str + str_cmd_inf->pos)
		- after_last_semicolon;
	if ((new_elem = ft_lstnew(after_last_semicolon,
					cur_cmd_size + 1)) == NULL)
		return (ft_lstfree(&cmd_lst));
	((char*)new_elem->content)[cur_cmd_size] = '\0';
	ft_lstpush(&cmd_lst, new_elem);
	return (cmd_lst);
}
