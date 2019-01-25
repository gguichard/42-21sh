/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 12:26:08 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/25 17:47:29 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_VARS_H
# define EXPAND_VARS_H

# include "shell.h"

/*
** Alloue et retourne une nouvelle string avec les variables d'environnement
** etendues si une variable a un nom invalide une copie de son nom sera set
** dans var_error (penser a free). Retourne NULL en cas d'erreur.
*/
char	*expand_vars(const char *str, t_shell *shell, char **var_error);

/*
** Alloue et retourne une nouvelle string avec le home expand si present.
** Retourne NULL en cas d'erreur.
*/
char	*expand_home(const char *str, t_shell *shell,
		int remove_home_ending_slash);

#endif
