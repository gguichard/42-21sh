/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 13:33:39 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/23 16:18:58 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include "libft.h"
# include <term.h>

# define ERR_PREFIX "21sh"
# define MAX_ESC_SEQ_BYTES 8
# define CMDLINE_DEF_CAPACITY 1024

typedef enum			e_prompt
{
	PROMPT_DEF = 0,
	PROMPT_ESCAPED,
	PROMPT_QUOTE,
	PROMPT_DQUOTE,
	PROMPT_HEREDOC
}						t_prompt;

typedef struct			s_select
{
	size_t				begin;
	size_t				end;
	char				*clipboard;
}						t_select;

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
	char				*def_line;
	t_prompt			prompt;
	char				*multiline;
	size_t				capacity;
	size_t				size;
	size_t				cursor;
	size_t				col;
	size_t				row;
	size_t				rows;
	size_t				offset;
	int					ac_flag;
	int					visual_mode;
	t_select			select;
}						t_term;

typedef struct			s_history
{
	char				*content;
	struct s_history	*prev;
	struct s_history	*next;
}						t_history;

typedef struct			s_builtin
{
	char				*name;
	int					(*builtin_fun);
}						t_builtin;

typedef struct			s_shell
{
	int					argc;
	char				**argv;
	t_list				*env;
	t_list				*local;
	t_builtin			*builtins;
	int					last_status;
	t_term				term;
	t_history			*history;
	t_history			*history_off;
	t_list				*fork_pids;
}						t_shell;

void					handle_signal(int sig);
int						init_shell(t_shell *shell, int argc, char **argv
		, char **environ);

t_list					*parse_env(char **environ);

void					show_prompt(t_shell *shell);

int						builtin_cd(t_shell *shell, int argc, char **argv);

#endif
