/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 13:33:39 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/06 01:14:12 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "libft.h"
# include <term.h>

# define MAX_ESC_SEQ_BYTES 8
# define CMDLINE_CAPACITY 1024

typedef struct			s_term
{
	struct termios		default_term;
	struct termios		curr_term;
	int					legacy_mode;
	struct winsize		winsize;
	int					esc_seq;
	char				seq[MAX_ESC_SEQ_BYTES];
	int					seq_off;
	char				*line;
	size_t				capacity;
	size_t				size;
	size_t				cursor;
	size_t				offset;
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

void					show_prompt(t_shell *shell);

#endif
