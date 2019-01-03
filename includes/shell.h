/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 13:33:39 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/03 14:29:15 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "libft.h"

typedef struct	s_var
{
	char		*key;
	char		*value;
}				t_var;

typedef struct	s_shell
{
	int			argc;
	char		**argv;
	t_list		*env;
	t_list		*local;
	int			last_exit;
}				t_shell;

#endif
