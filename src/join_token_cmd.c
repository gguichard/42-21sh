/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_token_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 11:46:53 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/25 16:50:53 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include "libft.h"
#include "expand_vars.h"
#include "apply_escape.h"
#include "shell.h"
#include "cmd_inf.h"
#include "token_inf.h"
#include "redirect_inf.h"
#include "join_token_cmd.h"

static int			add_arg(t_cmd_inf *cmd_inf, const char *arg, t_shell *shell)
{
	t_list	*new_elem;
	char	*home_expanded_arg;
	char	*escaped_arg;

	if ((home_expanded_arg = expand_home(arg, shell)) == NULL)
		return (0);
	if ((escaped_arg = apply_escape(home_expanded_arg)) == NULL)
	{
		free(home_expanded_arg);
		return (0);
	}
	free(home_expanded_arg);
	if ((new_elem = ft_lstnew(escaped_arg,
					sizeof(char) * (ft_strlen(escaped_arg) + 1))) == NULL)
	{
		free(escaped_arg);
		return (0);
	}
	ft_lstpush(&(cmd_inf->arg_lst), new_elem);
	free(escaped_arg);
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
		t_list *tkl_ropt)
{
	long int		tmp_fd;
	char			*tmp_fd_end;

	new_red->to_fd = FD_NOTSET;
	if (get_tk(tkl_ropt)->type == TK_STR_OPT
			&& ft_strequ(get_tk(tkl_ropt)->token, "&"))
	{
		new_red->to_fd = FD_AMPERSAND;
		if ((tkl_ropt = tkl_ropt->next) == NULL)
			return (0);
	}
	if (get_tk(tkl_ropt)->type == TK_STR_OPT)
	{
		new_red->to_word = NULL;
		if ((tmp_fd = ft_strtol(get_tk(tkl_ropt)->token + 1, &tmp_fd_end, 10))
				> INT_MAX)
			new_red->to_fd = FD_ERROR;
		else if (tmp_fd_end == (get_tk(tkl_ropt)->token + 1))
			new_red->to_fd = FD_DEFAULT;
		else
			new_red->to_fd = tmp_fd;
		return ((new_red->close_to_fd = (*tmp_fd_end == '-')) ? 1 : 1);
	}
	new_red->close_to_fd = 0;
	return ((new_red->to_word = ft_strdup(get_tk(tkl_ropt)->token)) != NULL);
}

static int			add_redirect_inf_to_cmd(t_cmd_inf *cmd_inf,
		t_token_inf *tk_lopt, t_token_inf *tk_ope, t_list *tkl_ropt)
{
	t_redirect_inf	new_red;
	t_list			*new_elem;

	if (tkl_ropt == NULL)
		return (0);
	process_lopt_redirect(&new_red, tk_lopt);
	new_red.red_type = redirection_str_to_type(tk_ope->token);
	if (!process_ropt_redirect(&new_red, tkl_ropt))
		return (0);
	new_red.heredoc = NULL;
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

static int			process_tk_ope(t_cmd_inf *cmd_inf, t_list **token_lst,
		t_shell *shell)
{
	if (get_tk(*token_lst)->token[0] == '<'
			|| get_tk(*token_lst)->token[0] == '>')
	{
		if ((*token_lst)->next == NULL
				|| !add_redirect_inf_to_cmd(cmd_inf, NULL,
					get_tk(*token_lst),
					(*token_lst)->next))
			return (del_cur_cmd(cmd_inf));
		*token_lst = get_ropt_elem((*token_lst)->next);
	}
	else if (ft_strequ(get_tk(*token_lst)->token, "|"))
	{
		if ((cmd_inf->pipe_cmd = (t_cmd_inf*)malloc(sizeof(t_cmd_inf)))
				== NULL)
			return (del_cur_cmd(cmd_inf));
		*token_lst = (*token_lst)->next;
		if (set_cur_cmd(cmd_inf->pipe_cmd, token_lst, shell))
			return (1);
		else
			return (del_cur_cmd(cmd_inf));
	}
	return (-1);
}

static int			process_tk(t_cmd_inf *cmd_inf, t_list **token_lst,
		t_shell *shell)
{
	if (get_tk(*token_lst)->type == TK_WORD)
	{
		if (!add_arg(cmd_inf, get_tk(*token_lst)->token, shell))
			return (del_cur_cmd(cmd_inf));
	}
	else if (get_tk(*token_lst)->type == TK_OPE)
	{
		return (process_tk_ope(cmd_inf, token_lst, shell));
	}
	else if (get_tk(*token_lst)->type == TK_NUM_OPT)
	{
		if ((*token_lst)->next == NULL || (*token_lst)->next->next == NULL
				|| !add_redirect_inf_to_cmd(cmd_inf, get_tk(*token_lst),
					get_tk((*token_lst)->next),
					(*token_lst)->next->next))
			return (del_cur_cmd(cmd_inf));
		*token_lst = get_ropt_elem((*token_lst)->next->next);
	}
	else if (get_tk(*token_lst)->type == TK_CMD_SEP)
		return (1);
	return (-1);
}

int					set_cur_cmd(t_cmd_inf *cmd_inf, t_list **token_lst,
		t_shell *shell)
{
	int		tmp_ret;

	cmd_inf->arg_lst = NULL;
	cmd_inf->pipe_cmd = NULL;
	cmd_inf->redirect_lst = NULL;
	while (*token_lst != NULL)
	{
		if ((tmp_ret = process_tk(cmd_inf, token_lst, shell)) != -1)
			return (tmp_ret);
		if (*token_lst != NULL)
			*token_lst = (*token_lst)->next;
	}
	return (1);
}

t_list				*join_token_cmd(t_list *token_lst, t_shell *shell)
{
	t_list		*cmd_lst;
	t_list		*new_elem;
	t_cmd_inf	cur_cmd;

	cmd_lst = NULL;
	while (token_lst != NULL)
	{
		if (!set_cur_cmd(&cur_cmd, &token_lst, shell))
		{
			ft_lstdel(&cmd_lst, del_cmd);
			return (NULL);
		}
		else
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
