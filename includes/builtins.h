/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/28 16:52:57 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/28 16:53:35 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "shell.h"

void	set_dir_to_canonical_form(char *dir);
int		builtin_cd(t_shell *shell, int argc, char **argv);
int		builtin_exit(t_shell *shell, int argc, char **argv);
int		builtin_echo(t_shell *shell, int argc, char **argv);

#endif
