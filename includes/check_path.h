/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_path.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 08:10:24 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/22 12:16:24 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECK_PATH_H
# define CHECK_PATH_H

# include "error.h"

# define DS_IS_A_FILE	1
# define DS_IS_A_DIR	2
# define DS_IS_ANY		3

/*
** Retourne ERRC_NOERROR (qui vaut 0) si le chemin pointe vers
** un dossier valide pour cd et a les droits demandes, ou
** un code d'erreur sinon.
*/
t_error		check_dir_for_cd_and_right(const char *path, int right);

/*
** Retourne ERRC_NOERROR (qui vaut 0) si le dossier du fichier pointe par
** le chemin est valide pour cd et a les droits demandes, ou
** un code d'erreur sinon.
*/
t_error		check_dir_of_file_for_cd_and_right(const char *path, int right);

/*
** Retourne ERRC_NOERROR (qui vaut 0) si le chemin pointe vers
** un fichier qui a les droits demandes et correspond au type de dir_stat, ou
** un code d'erreur sinon.
*/
t_error		check_file_for_right(const char *path, int dir_stat, int right);

#endif
