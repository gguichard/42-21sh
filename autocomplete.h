/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 12:39:06 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/05 15:39:21 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOCOMPLETE_H
# define AUTOCOMPLETE_H

typedef struct	s_ac_inf
{
	int			is_dir;
	char		*suff;
	size_t		suff_len;
}				t_ac_inf;

/*
** Malloc et retourne une chaine de caractere a ajouter a word pour arriver au
** plus grand prefix commun des mots possibles, une chaine vide s'il n'y
** en a pas et NULL s'il y a eu une erreur. Le parametre is_a_cmd doit valoir
** true si le word doit etre un executable, false s'il peut etre un fichier
** quelconque.
*/
char	*autocomplet_word(const char *word, int is_a_cmd, const char *path);

#endif
