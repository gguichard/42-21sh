/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 13:33:39 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/03 20:56:20 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "libft.h"
# include <term.h>

typedef struct		s_var
{
	char			*key;
	char			*value;
}					t_var;

typedef struct		s_shell
{
	int				argc;
	char			**argv;
	t_list			*env;
	t_list			*local;
	int				last_status;
	struct termios	default_term;
	struct termios	curr_term;
	int				legacy_mode;
}					t_shell;

t_list				*parse_env(char **environ);

#endif
