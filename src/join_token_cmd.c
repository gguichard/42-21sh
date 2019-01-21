/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_token_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 11:46:53 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/21 14:29:14 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include "libft.h"
#include "cmd_inf.h"
#include "token_inf.h"
#include "redirect_inf.h"
#include "join_token_cmd.h"

static t_token_inf	*get_tk(t_list *lst)
{
	if (lst != NULL)
		return ((t_token_inf*)(lst->content));
	else
		return (NULL);
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

static void			process_lopt_redirect(t_redirect_inf *new_red,
		t_token_inf *tk_lopt)
{
	long int	tmp_fd;

	if (tk_lopt == NULL)
		new_red->from_fd = FD_DEFAULT;
	else if (ft_strequ(tk_lopt->token, "&"))
		new_red->from_fd = FD_AMPERSAND;
	else
	{
		if ((tmp_fd = ft_strtol(tk_lopt->token, NULL, 10)) > INT_MAX)
			new_red->from_fd = FD_ERROR;
		else
			new_red->from_fd = tmp_fd;
	}
}

static int			process_ropt_redirect(t_redirect_inf *new_red,
		t_token_inf *tk_ropt)
{
	long int		tmp_fd;
	char			*tmp_fd_end;

	if (tk_ropt->type == TK_STR_OPT)
	{
		new_red->to_word = NULL;
		if ((tmp_fd = ft_strtol(tk_ropt->token + 1, &tmp_fd_end, 10)) > INT_MAX)
			new_red->to_fd = FD_ERROR;
		else if (tmp_fd_end == (tk_ropt->token + 1))
			new_red->to_fd = FD_DEFAULT;
		else
			new_red->to_fd = tmp_fd;
		new_red->close_to_fd = (*tmp_fd_end == '-');
	}
	else
	{
		new_red->to_fd = FD_NOTSET;
		new_red->close_to_fd = 0;
		if ((new_red->to_word = ft_strdup(tk_ropt->token)) == NULL)
			return (0);
	}
	return (1);
}

static int			add_redirect_inf_to_cmd(t_cmd_inf *cmd_inf,
		t_token_inf *tk_lopt, t_token_inf *tk_ope, t_token_inf *tk_ropt)
{
	t_redirect_inf	new_red;
	t_list			*new_elem;

	if (tk_ropt == NULL)
		return (0);
	process_lopt_redirect(&new_red, tk_lopt);
	new_red.red_type = redirection_str_to_type(tk_ope->token);
	if (!process_ropt_redirect(&new_red, tk_ropt))
		return (0);
	if ((new_elem = ft_lstnew(&new_red, sizeof(t_redirect_inf))) == NULL)
	{
		free(new_red.to_word);
		return (0);
	}
	ft_lstpush(&(cmd_inf->redirect_lst), new_elem);
	return (1);
}

static t_list		*get_ropt_elem(t_list *tk_lst)
{
	if (get_tk(tk_lst)->type == TK_STR_OPT
			&& ft_strequ(get_tk(tk_lst)->token, "&"))
		return (tk_lst->next);
	else
		return (tk_lst);
}

static int			del_cur_cmd(t_cmd_inf *cmd_inf)
{
	ft_lstfree(&(cmd_inf->arg_lst));
	ft_lstdel(&(cmd_inf->redirect_lst), del_redirect);
	return (0);
}

static int			process_tk_ope(t_cmd_inf *cmd_inf, t_list **token_lst)
{
	if (get_tk(*token_lst)->token[0] == '<'
			|| get_tk(*token_lst)->token[0] == '>')
	{
		if ((*token_lst)->next == NULL
				|| !add_redirect_inf_to_cmd(cmd_inf, NULL,
					get_tk(*token_lst),
					get_tk(get_ropt_elem((*token_lst)->next))))
			return (del_cur_cmd(cmd_inf));
		*token_lst = get_ropt_elem((*token_lst)->next);
	}
	else if (ft_strequ(get_tk(*token_lst)->token, "|"))
	{
		if ((cmd_inf->pipe_cmd = (t_cmd_inf*)malloc(sizeof(t_cmd_inf)))
				== NULL)
			return (del_cur_cmd(cmd_inf));
		*token_lst = (*token_lst)->next;
		if (set_cur_cmd(cmd_inf->pipe_cmd, token_lst))
			return (1);
		else
			return (del_cur_cmd(cmd_inf));
	}
	return (-1);
}

static int			process_tk(t_cmd_inf *cmd_inf, t_list **token_lst)
{
	if (get_tk(*token_lst)->type == TK_WORD)
	{
		if (!add_arg(cmd_inf, get_tk(*token_lst)->token))
			return (del_cur_cmd(cmd_inf));
	}
	else if (get_tk(*token_lst)->type == TK_OPE)
	{
		return (process_tk_ope(cmd_inf, token_lst));
	}
	else if (get_tk(*token_lst)->type == TK_NUM_OPT)
	{
		if ((*token_lst)->next == NULL || (*token_lst)->next->next == NULL
				|| !add_redirect_inf_to_cmd(cmd_inf, get_tk(*token_lst),
					get_tk((*token_lst)->next),
					get_tk(get_ropt_elem((*token_lst)->next->next))))
			return (del_cur_cmd(cmd_inf));
		*token_lst = get_ropt_elem((*token_lst)->next->next);
	}
	else if (get_tk(*token_lst)->type == TK_CMD_SEP)
		return (1);
	return (-1);
}

int					set_cur_cmd(t_cmd_inf *cmd_inf, t_list **token_lst)
{
	int		tmp_ret;

	cmd_inf->arg_lst = NULL;
	cmd_inf->pipe_cmd = NULL;
	cmd_inf->redirect_lst = NULL;
	while (*token_lst != NULL)
	{
		if ((tmp_ret = process_tk(cmd_inf, token_lst)) != -1)
			return (tmp_ret);
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
			ft_lstdel(&cmd_lst, del_cmd);
			return (NULL);
		}
		else if (cur_cmd.arg_lst != NULL)
		{
			if ((new_elem = ft_lstnew(&cur_cmd, sizeof(t_cmd_inf))) == NULL)
			{
				ft_lstdel(&cmd_lst, del_cmd);
				return (NULL);
			}
			ft_lstpush(&cmd_lst, new_elem);
		}
		if (token_lst != NULL)
			token_lst = token_lst->next;
	}
	return (cmd_lst);
}
