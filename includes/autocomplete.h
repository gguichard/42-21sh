/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 12:39:06 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/10 16:29:45 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOCOMPLETE_H
# define AUTOCOMPLETE_H

# include <dirent.h>
# include <sys/stat.h>
# include "libft.h"
# include "shell.h"

typedef enum		e_ac_suff_type
{
	ACS_TYPE_NOTHING,
	ACS_TYPE_DIR,
	ACS_TYPE_FILE
}					t_ac_suff_type;

typedef struct		s_ac_rdir_inf
{
	DIR				*dir;
	char			*dir_to_use;
	char			*file_word;
	char			*cur_file_path;
	char			*cur_file_name;
	size_t			file_word_len;
	struct stat		stat_buf;
	int				need_to_be_cmd;
	int				can_be_dir;
}					t_ac_rdir_inf;

typedef struct		s_ac_suff_inf
{
	t_list			*choices;
	t_ac_suff_type	suff_type;
	char			*suff;
	long long		suff_len;
}					t_ac_suff_inf;

/*
** Free le contenu de l'ac_suff_inf passe en parametre puis free la structure
** en elle-meme.
** Renvoie toujours NULL.
*/
void				*delete_ac_suff_inf(t_ac_suff_inf *acs);

/*
** Malloc et retourne une structure contenant une chaine de caractere a ajouter
** a word pour arriver au plus grand prefix commun des mots possibles, une
** chaine vide s'il n'y en a pas et NULL s'il y a eu une erreur.
** Les variable is_dir et is_file sont set si le suffix represente
** respectivement un dossier ou un fichier (pour l'ajout d'un slash ou
** d'un espace final).
** Le parametre is_a_cmd doit valoir true si le word doit etre un executable,
** false s'il peut etre un fichier quelconque.
*/
t_ac_suff_inf		*autocomplet_word(const char *word, int is_a_cmd,
		const char *path, t_builtin **builtin_tab);

#endif
