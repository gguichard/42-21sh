/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_path.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 08:10:24 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/03 16:50:54 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECK_PATH_H
# define CHECK_PATH_H

# include "error.h"

/*
** Retourne ERRC_NOERROR (qui vaut 0) si le chemin pointe vers
** un dossier valide pour cd, ou un code d'erreur sinon.
*/
t_error		check_dir_for_cd(const char *path);

/*
** Retourne ERRC_NOERROR (qui vaut 0) si le dossier du fichier pointe par
** le chemin est valide pour cd, ou un code d'erreur sinon.
*/
t_error		check_dir_of_file_for_cd(const char *path);

/*
** Retourne ERRC_NOERROR (qui vaut 0) si le chemin pointe vers
** un fichier executable, ou un code d'erreur sinon.
*/
t_error		check_file_for_exec(const char *path);

#endif
