/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 13:12:56 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/21 10:46:31 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "error.h"

char	*search_binary(const char *path, const char *binary, t_error *error);

void	execute_cmd(t_shell *shell, t_list *all_sub_cmd);

#endif
