/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 12:39:06 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/28 13:25:52 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUTOCOMPLETE_H
# define AUTOCOMPLETE_H

# include <dirent.h>
# include <sys/stat.h>
# include "libft.h"
# include "str_cmd_inf.h"
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

void				strlist_insert_sort(t_list **lst, t_list *elem);

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
** La variable suff_type contient le type du suffix, un dossier ou un
** fichier (pour l'ajout d'un slash ou d'un espace final).
** Le parametre is_a_cmd doit valoir true si le word doit etre un executable,
** false s'il peut etre un fichier quelconque.
*/
t_ac_suff_inf		*autocomplete_word(t_shell *shell, const char *word
		, int is_a_cmd, t_builtin **builtin_tab);

t_ac_suff_inf		*autocomplete_cmdline(t_str_cmd_inf *scmd, t_shell *shell
		, t_builtin **builtins);

/*
** Alloue et retourne un nouvel element representant un choix valide pour
** l'autocompletion.
*/
t_list				*make_new_choice(t_ac_rdir_inf *acrd);

/*
** Remplie les inforamtions du prochain fichier a lire present dans le dossier
** contenu dans le t_ac_rdir_inf. Retourne 1 si un fichier a ete trouve et 0
** sinon. Met le suffix du t_ac_suff_inf a NULL s'il y a eu une erreur.
*/
int					readdir_to_dirent(t_ac_rdir_inf *acrd, t_ac_suff_inf *acs);

/*
** Initialise le contenu du t_ac_rdir_inf avec les informations passes en
** parametre. Retourne 0 si l'initialisation rate, 1 si elle reussi.
*/
int		init_ac_rdir(const char *word, t_ac_rdir_inf *acrd,
		int need_to_be_cmd, int can_be_dir);

/*
** Supprime le contenu du t_ac_rdir_inf.
*/
void	delete_ac_rdir(t_ac_rdir_inf *acrd);

#endif
