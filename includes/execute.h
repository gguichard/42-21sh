/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 13:12:56 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/24 18:23:05 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include <unistd.h>
# include "shell.h"
# include "libft.h"
# include "cmd_inf.h"
# include "redirect_inf.h"
# include "error.h"

typedef struct	s_pipe
{
	int			is_leftmost;
	int			is_rightmost;
	int			fildes[2];
	int			in_fd;
	int			out_fd;
	t_cmd_inf	*cmd_inf;
}				t_pipe;

void			child_exec_cmd_inf(t_shell *shell, t_cmd_inf *cmd_inf
		, const char *bin_path, char **args);
void			execute_all(t_shell *shell, t_list *all_sub_cmd);
void			execute_single_cmd(t_shell *shell, t_cmd_inf *cmd_inf
		, const char *path);
void			execute_pipeline(t_shell *shell, t_cmd_inf *cmd_inf
		, const char *path);

/*
 ** UTILS.
 */
char			*search_binary(const char *path, const char *binary
		, t_error *error);
char			**arg_lst_to_tab(t_list *arg_lst);

/*
 ** REDIRECTIONS.
 */
void			process_redir(t_cmd_inf *cmd_inf);
int				fork_redirect(t_cmd_inf *cmd_inf);
int				redirect_output(t_redirect_inf *redirect_inf);
int				redirect_input(t_redirect_inf *redirect_inf);

/*
 ** COMMANDS.
 */
char			*get_cmd_inf_path(t_cmd_inf *cmd_inf, const char *path
		, t_error *error);

#endif
