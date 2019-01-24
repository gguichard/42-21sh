/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 12:26:08 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/24 12:36:26 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_VARS_H
# define EXPAND_VARS_H

# include "shell.h"

/*
** Alloue et retourne une nouvelle string avec les variables d'environnement
** etendues.
*/
char	*expand_vars(const char *str, t_shell *shell);

#endif
