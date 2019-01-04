/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_path_to_tab.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 14:18:42 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/04 14:46:31 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONVERT_PATH_TO_TAB_H
# define CONVERT_PATH_TO_TAB_H

/*
** Modifie path et malloc un tableau contenant les differentes parties du path.
** Ce tableau contiendra des pointeurs sur path, donc ce dernier doit rester
** valide. Retourne le tableau ou NULL en cas d'erreur.
*/
char	**convert_path_to_tab(char *path);

#endif
