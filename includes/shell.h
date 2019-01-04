/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 13:33:39 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/04 15:46:31 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "libft.h"
# include <term.h>

# define MAX_ESC_SEQ_BYTES 8
# define CMDLINE_CAPACITY 1024

typedef struct			s_cmdline
{
	char				key;
	struct s_cmdline	*prev;
	struct s_cmdline	*next;
}						t_cmdline;

typedef struct			s_term
{
	struct termios		default_term;
	struct termios		curr_term;
	int					legacy_mode;
	int					esc_seq;
	char				seq[MAX_ESC_SEQ_BYTES];
	int					seq_off;
	char				*cmdline;
	size_t				cmdline_cap;
	size_t				cmdline_siz;
	size_t				cursor;
}						t_term;

typedef struct			s_shell
{
	int					argc;
	char				**argv;
	t_list				*env;
	t_list				*local;
	int					last_status;
	t_term				term;
}						t_shell;

t_list					*parse_env(char **environ);

#endif
