/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 09:50:35 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/09 10:17:47 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
# define HISTORY_H

# include "shell.h"

void					clear_history_entry(t_history *elem);
void					clear_history(t_shell *shell);
void					add_history_entry(t_shell *shell, const char *command);
const char				*peek_history_prev(t_shell *shell);
const char				*peek_history_next(t_shell *shell);

/*
** HOOKS.
*/
void					history_up(t_term *term);
void					history_down(t_term *term);

#endif
