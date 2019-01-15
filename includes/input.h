/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 20:36:02 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/15 10:44:52 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include "shell.h"
# include "autocomplete.h"

# define EOT_KEY 4
# define BACKSPACE_KEY 127
# define ESC_SEQ_LEFT "\033[D"
# define ESC_SEQ_RIGHT "\033[C"
# define ESC_SEQ_UP "\033[A"
# define ESC_SEQ_DOWN "\033[B"
# define ESC_SEQ_PAGEUP "\033[5~"
# define ESC_SEQ_PAGEDOWN "\033[6~"
# define ESC_DEL_KEY "\033[3~"
# define ESC_HOME_KEY "\033[H"
# define ESC_END_KEY "\033[F"
# define ESC_SHIFT_LEFT "\033[1;2D"
# define ESC_SHIFT_RIGHT "\033[1;2C"
# define ESC_SHIFT_UP "\033[1;2A"
# define ESC_SHIFT_DOWN "\033[1;2B"
# define ESC_FN_F1 "\033OP"

typedef struct	s_seq
{
	char		*str;
	int			(*f)(t_shell *shell, t_term *term);
}				t_seq;

typedef struct	s_ac_format
{
	int			elems;
	int			col_width;
	int			max_col;
	int			max_row;
	t_list		**cols;
}				t_ac_format;

/*
** SETUP TERM.
*/
int				setup_term(t_shell *shell);
int				reset_term(t_shell *shell);
int				update_winsize(t_term *term);

/*
** COMMAND LINE.
*/
size_t			get_rows(t_term *term);

int				handle_command(t_shell *shell);
int				wait_for_command(t_shell *shell);
size_t			get_max_col(t_term *term);
void			reset_cmdline(t_shell *shell);
int				realloc_cmdline(t_term *term);
void			refresh_cmdline(t_shell *shell, t_term *term);
void			print_cmdline(t_shell *shell, t_term *term);
void			insert_cmdline(t_shell *shell, t_term *term, char key);
int				handle_key_mode(t_shell *shell, t_term *term, char key);

/*
** ESCAPE sequences.
*/
const t_seq		*get_valid_esc_sequence(t_term *term);
int				handle_esc_key(t_shell *shell, t_term *term, char key);
void			handle_esc_sequence(t_shell *shell, t_term *term
		, const t_seq *seq);

/*
** INSERT MODE.
*/
void			init_ac_format(t_ac_format *fmt, t_list *lst, t_term *term);
t_ac_format		*ac_get_format(t_list *lst, t_term *term);
void			ac_print_list(t_list *lst, t_term *term);
void			ac_append(t_shell *shell, t_term *term, t_ac_suff_inf *result);
int				handle_ac(t_shell *shell, t_term *term);
int				handle_eot_key(t_shell *shell, t_term *term);
int				handle_bs_key(t_shell *shell, t_term *term);
int				handle_del_key(t_shell *shell, t_term *term);
int				handle_key(t_shell *shell, t_term *term, char key);

/*
** VISUAL MODE.
*/
int				vm_toggle(t_shell *shell, t_term *term);
int				vm_copy_hook(t_shell *shell, t_term *term, int cut);
int				vm_paste_hook(t_shell *shell, t_term *term, int before_cursor);
int				handle_vm_key(t_shell *shell, t_term *term, char key);

/*
** HOOKS.
*/
int				move_cursor_left(t_shell *shell, t_term *term);
int				move_cursor_right(t_shell *shell, t_term *term);
int				move_cursor_home(t_shell *shell, t_term *term);
int				move_cursor_end(t_shell *shell, t_term *term);
int				move_cursor_prev_word(t_shell *shell, t_term *term);
int				move_cursor_next_word(t_shell *shell, t_term *term);
int				move_cursor_up(t_shell *shell, t_term *term);
int				move_cursor_down(t_shell *shell, t_term *term);

#endif
