/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 12:26:08 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/24 15:50:58 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_VARS_H
# define EXPAND_VARS_H

# include "shell.h"

/*
** Alloue et retourne une nouvelle string avec les variables d'environnement
** etendues si une variable a un nom invalide une copie de son nom sera set
** dans var_error (penser a free).
*/
char	*expand_vars(const char *str, t_shell *shell, char **var_error);

#endif
