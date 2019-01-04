/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 20:36:02 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/04 23:18:33 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_H
# define INPUT_H

# include "shell.h"

# define ESC_SEQ_LEFT "\e[D"
# define ESC_SEQ_RIGHT "\e[C"
# define ESC_SEQ_UP "\e[A"
# define ESC_SEQ_DOWN "\e[B"
# define ESC_DEL_KEY "\e[3~"

/*
** SETUP TERM.
*/
int		setup_term(t_shell *shell);
int		reset_term(t_shell *shell);

/*
** ESCAPE sequences.
*/
char	*get_valid_esc_seq(t_term *term);
int		append_esc_seq(t_term *term, char key);

/*
** COMMAND LINE.
*/
int		setup_cmdline(t_term *term);
void	update_winsize(t_term *term);
int		wait_for_command(t_shell *shell);
void	refresh_cmdline(t_term *term);
void	print_cmdline(t_term *term, char key);
int		realloc_cmdline(t_term *term);
void	append_cmdline(t_term *term, char key);
void	handle_esc_key(t_term *term, const char *seq);

/*
** HOOKS.
*/
void	move_cursor_right_col(t_term *term);
void	move_cursor_left(t_term *term);
void	move_cursor_right(t_term *term);
void	handle_bs_key(t_term *term);
void	handle_del_key(t_term *term);

#endif
