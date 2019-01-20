/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_token_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 11:46:53 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/18 16:36:19 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "cmd_inf.h"
#include "token_inf.h"
#include "join_token_cmd.h"

static t_token_inf	*get_tk(t_list *lst)
{
	return ((t_token_inf*)(lst->content));
}

static int			add_arg(t_cmd_inf *cmd_inf, const char *arg)
{
	t_list	*new_elem;

	if ((new_elem = ft_lstnew(arg,
					sizeof(char) * (ft_strlen(arg) + 1))) == NULL)
		return (0);
	ft_lstpush(&(cmd_inf->arg_lst), new_elem);
	return (1);
}

static int			set_cur_cmd(t_cmd_inf *cmd_inf, t_list **token_lst)
{
	cmd_inf->arg_lst = NULL;
	cmd_inf->pipe_cmd = NULL;
	cmd_inf->redirect_lst = NULL;
	while (*token_lst != NULL)
	{
		if (get_tk(*token_lst)->type == TK_WORD)
		{
			if (!add_arg(cmd_inf, get_tk(*token_lst)->token))
			{
				ft_lstfree(&(cmd_inf->arg_lst));
				return (0);
			}
		}
		else if (get_tk(*token_lst)->type == TK_OPE)
		{
			if (get_tk(*token_lst)->token[0] == '<'
					|| get_tk(*token_lst)->token[0] == '>')
			{
				*token_lst = (*token_lst)->next;
			}
			else if (ft_strequ(get_tk(*token_lst)->token, "|"))
			{
				if ((cmd_inf->pipe_cmd = (t_cmd_inf*)malloc(sizeof(t_cmd_inf)))
						== NULL)
				{
					ft_lstfree(&(cmd_inf->arg_lst));
					return (0);
				}
				*token_lst = (*token_lst)->next;
				if (set_cur_cmd(cmd_inf->pipe_cmd, token_lst))
				{
					return (1);
				}
				else
				{
					ft_lstfree(&(cmd_inf->arg_lst));
					return (0);
				}
			}
		}
		else if (get_tk(*token_lst)->type == TK_CMD_SEP)
		{
			return (1);
		}
		if (*token_lst != NULL)
			*token_lst = (*token_lst)->next;
	}
	return (1);
}

t_list				*join_token_cmd(t_list *token_lst)
{
	t_list		*cmd_lst;
	t_list		*new_elem;
	t_cmd_inf	cur_cmd;

	cmd_lst = NULL;
	while (token_lst != NULL)
	{
		if (!set_cur_cmd(&cur_cmd, &token_lst))
		{
			ft_lstdel(&token_lst, del_cmd);
			return (NULL);
		}
		else if (cur_cmd.arg_lst != NULL)
		{
			if ((new_elem = ft_lstnew(&cur_cmd, sizeof(t_cmd_inf))) == NULL)
			{
				ft_lstdel(&token_lst, del_cmd);
				return (NULL);
			}
			ft_lstpush(&cmd_lst, new_elem);
		}
		if (token_lst != NULL)
			token_lst = token_lst->next;
	}
	return (cmd_lst);
}
