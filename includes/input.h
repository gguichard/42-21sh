/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 20:36:02 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/04 10:15:13 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERM_H
# define TERM_H

# include "shell.h"

# define ESC_SEQ_LEFT "\e[D"
# define ESC_SEQ_RIGHT "\e[C"
# define ESC_SEQ_UP "\e[A"
# define ESC_SEQ_DOWN "\e[B"

/*
** SETUP TERM.
*/
int		setup_term(t_shell *shell);
int		reset_term(t_shell *shell);

/*
** ESCAPE sequences.
*/
int		is_valid_esc_seq(t_shell *shell);
int		append_esc_seq(t_shell *shell, char key);

/*
** COMMAND LINE.
*/
int		wait_for_command(t_shell *shell);
void	append_char_cmdline(t_shell *shell, char key);
char	*get_cmdline(t_shell *shell);

#endif
