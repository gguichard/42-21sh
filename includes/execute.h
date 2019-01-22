/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 13:12:56 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/22 12:07:04 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "shell.h"
# include "libft.h"
# include "cmd_inf.h"
# include "redirect_inf.h"
# include "error.h"

char	*search_binary(const char *path, const char *binary, t_error *error);

char	**arg_lst_to_tab(t_list *arg_lst);

int		fork_redirect(t_cmd_inf *cmd_inf);
int		redirect_output(t_redirect_inf *redirect_inf);
int		redirect_input(t_redirect_inf *redirect_inf);

void	execute_all(t_shell *shell, t_list *all_sub_cmd);

#endif
