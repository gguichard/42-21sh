/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 12:39:06 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/07 10:45:21 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOCOMPLETE_H
# define AUTOCOMPLETE_H

# include <dirent.h>
# include <sys/stat.h>

typedef struct		s_ac_rdir_inf
{
	DIR				*dir;
	struct dirent	*dirent;
	char			*dir_to_use;
	char			*file_word;
	char			*cur_file_path;
	size_t			file_word_len;
	struct stat		stat_buf;
	int				need_to_be_cmd;
	int				can_be_dir;
}					t_ac_rdir_inf;

typedef struct		s_ac_suff_inf
{
	int				is_dir;
	char			*suff;
	long long		suff_len;
}					t_ac_suff_inf;

/*
** Malloc et retourne une chaine de caractere a ajouter a word pour arriver au
** plus grand prefix commun des mots possibles, une chaine vide s'il n'y
** en a pas et NULL s'il y a eu une erreur. Le parametre is_a_cmd doit valoir
** true si le word doit etre un executable, false s'il peut etre un fichier
** quelconque.
*/
char				*autocomplet_word(const char *word, int is_a_cmd,
		const char *path);

#endif
