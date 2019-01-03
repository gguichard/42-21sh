/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 17:53:02 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/03 18:18:58 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VARS_H
# define VARS_H

# include "shell.h"

void		free_var(void *content, size_t content_size);
t_var		*get_var(t_list *lst, const char *key);
int			create_var(t_list **lst, const char *key, const char *value);
int			update_var(t_list **lst, const char *key, const char *value);
int			unset_var(t_list **lst, const char *key);

char		*get_shell_var(t_shell *shell, const char *key);

#endif
