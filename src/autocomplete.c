/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 09:10:18 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/10 15:47:28 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "libft.h"
#include "convert_path_to_tab.h"
#include "shell.h"
#include "autocomplete.h"

/*
** Renvoie le nombre de premiers caracteres identiques entre les deux strings.
*/

static size_t	count_same_char(const char *str1, const char *str2)
{
	int		count;

	count = 0;
	while (str1[count] != '\0' && str1[count] == str2[count])
		++count;
	return (count);
}

/*
** Initialise le contenu du t_ac_rdir_inf avec les informations passes en
** parametre. Retourne 0 si l'initialisation rate, 1 si elle reussi.
*/

static int		init_ac_rdir(const char *word, t_ac_rdir_inf *acrd,
		int need_to_be_cmd, int can_be_dir)
{
	char			*last_slash;

	acrd->need_to_be_cmd = need_to_be_cmd;
	acrd->can_be_dir = can_be_dir;
	if ((last_slash = ft_strrchr(word, '/')) == NULL)
		acrd->dir_to_use = ft_strdup("./");
	else
		acrd->dir_to_use = ft_strndup(word, last_slash - word + 1);
	if (acrd->dir_to_use == NULL || (acrd->file_word = ft_strdup(
					last_slash == NULL ? word : last_slash + 1)) == NULL)
	{
		ft_memdel((void**)&(acrd->dir_to_use));
		return (0);
	}
	acrd->file_word_len = ft_strlen(acrd->file_word);
	acrd->dir = NULL;
	acrd->cur_file_path = NULL;
	acrd->cur_file_name = NULL;
	return (1);
}

/*
** Supprime le contenu du t_ac_rdir_inf.
*/

static void		delete_ac_rdir(t_ac_rdir_inf *acrd)
{
	if (acrd->dir != NULL)
	{
		closedir(acrd->dir);
		acrd->dir = NULL;
	}
	ft_memdel((void**)&(acrd->dir_to_use));
	ft_memdel((void**)&(acrd->file_word));
	ft_memdel((void**)&(acrd->cur_file_path));
}

/*
** Remplie les inforamtions du prochain fichier a lire present dans le dossier
** contenu dans le t_ac_rdir_inf. Retourne 1 si un fichier a ete trouve et 0
** sinon. Met le suffix du t_ac_suff_inf a NULL s'il y a eu une erreur.
*/

static int		readdir_to_dirent(t_ac_rdir_inf *acrd, t_ac_suff_inf *acs)
{
	struct dirent	*tmp_dirent;

	while ((tmp_dirent = readdir(acrd->dir)) != NULL)
	{
		acrd->cur_file_name = tmp_dirent->d_name;
		if ((acrd->cur_file_path = ft_strjoin(acrd->dir_to_use,
						acrd->cur_file_name)) == NULL)
		{
			acs->suff = NULL;
			return (0);
		}
		if (stat(acrd->cur_file_path, &(acrd->stat_buf)) == -1)
		{
			ft_memdel((void**)&(acrd->cur_file_path));
			continue ;
		}
		return (1);
	}
	return (0);
}

/*
** Retourne 1 si le fichier present dans le t_ac_rdir_inf peut etre une
** autocompletion valide, 0 sinon.
*/

static int		valid_file_for_ac(t_ac_rdir_inf *acrd)
{
	if (acrd->cur_file_name[0] != '.' || acrd->file_word[0] == '.')
	{
		if (!acrd->need_to_be_cmd
				|| (acrd->can_be_dir && S_ISDIR(acrd->stat_buf.st_mode))
				|| (S_ISREG(acrd->stat_buf.st_mode)
					&& access(acrd->cur_file_path, X_OK) == 0))
		{
			if (ft_strnequ(acrd->cur_file_name, acrd->file_word,
					acrd->file_word_len))
			{
				return (1);
			}
		}
	}
	return (0);
}

/*
** Remplie le t_ac_suff_inf avec les informations pour autocompleter depuis
** les informations de fichier contenues dans le  t_ac_rdir_inf.
** Si force_file_type est vrai et que l'autocompletion a un type, ce type sera
** force en tant que fichier.
** Retourne 0 si l'autocompletion est vide (aucun suffix et aucun type).
** Retourne 1 sinon.
*/

static int		build_ac_suff(t_ac_rdir_inf *acrd, t_ac_suff_inf *acs,
		int force_file_type)
{
	if (acs->suff_len == -1)
	{
		free(acs->suff);
		acs->suff_len = ft_strlen(acrd->cur_file_name + acrd->file_word_len);
		if ((acs->suff =
					(char*)malloc(sizeof(char) * (acs->suff_len + 1))) == NULL)
			acs->suff_len = 0;
		else
		{
			ft_memcpy(acs->suff, acrd->cur_file_name + acrd->file_word_len,
					acs->suff_len + 1);
			acs->suff_type = (force_file_type
					|| !S_ISDIR(acrd->stat_buf.st_mode))
				? ACS_TYPE_FILE : ACS_TYPE_DIR;
		}
	}
	else
	{
		acs->suff_type = ACS_TYPE_NOTHING;
		acs->suff_len = count_same_char(acrd->cur_file_name +
				acrd->file_word_len, acs->suff);
		acs->suff[acs->suff_len] = '\0';
	}
	return (!(acs->suff_len == 0 && acs->suff_len == ACS_TYPE_NOTHING));
}

/*
** Alloue et retourne un nouvel element representant un choix valide pour
** l'autocompletion.
*/

static t_list	*make_new_choice(t_ac_rdir_inf *acrd)
{
	char	*tmp_file_name;
	size_t	tmp_file_name_len;
	t_list	*new_elem;

	tmp_file_name_len = ft_strlen(acrd->cur_file_name);
	if ((tmp_file_name = (char*)malloc(sizeof(char) * (tmp_file_name_len
					+ (S_ISDIR(acrd->stat_buf.st_mode) ? 1 : 0)))) == NULL)
		return (NULL);
	ft_memcpy(tmp_file_name, acrd->cur_file_name, tmp_file_name_len + 1);
	if (S_ISDIR(acrd->stat_buf.st_mode))
	{
		ft_memcpy(tmp_file_name + tmp_file_name_len, "/", 2);
		++tmp_file_name_len;
	}
	new_elem = ft_lstnew(tmp_file_name, tmp_file_name_len);
	free(tmp_file_name);
	return (new_elem);
}

/*
** Remplie le t_ac_suff_inf avec les informations pour autocompleter depuis
** les informations de fichier contenues dans le  t_ac_rdir_inf.
** Retourne 0 si l'autocompletion est vide (aucun suffix et aucun type).
** Retourne 1 sinon.
*/

static int		try_ac_for_this_file(t_ac_rdir_inf *acrd, t_ac_suff_inf *acs)
{
	t_list	*new_elem;

	if (valid_file_for_ac(acrd))
	{
		if ((new_elem = make_new_choice(acrd)) != NULL)
			ft_lstadd(&(acs->choices), new_elem);
		if (acs->suff_len == -1 || !ft_strnequ(acrd->cur_file_name +
					acrd->file_word_len, acs->suff, acs->suff_len))
		{
			if (!build_ac_suff(acrd, acs, 0))
			{
				ft_memdel((void**)&(acrd->cur_file_path));
				return (0);
			}
		}
		else
			acs->suff_type = ACS_TYPE_NOTHING;
	}
	ft_memdel((void**)&(acrd->cur_file_path));
	return (1);
}

/*
** Remplie le t_ac_suff_inf avec les informations pour autocompleter depuis
** les informations de dossier contenues dans le  t_ac_rdir_inf.
*/

static void		autocomplete_with_infs(t_ac_rdir_inf *acrd, t_ac_suff_inf *acs)
{
	if ((acrd->dir = opendir(acrd->dir_to_use)) != NULL)
	{
		while (readdir_to_dirent(acrd, acs))
		{
			if (!try_ac_for_this_file(acrd, acs))
				break ;
		}
	}
}

/*
** Remplie le t_ac_suff_inf avec les informations pour autocompleter le word
** passe en parametre, si is_a_cmd est vrai le suffix ne peut etre qu'un
** executable ou un dossier.
*/

static void		autocomplet_from_wordpath(const char *word, int is_a_cmd,
		t_ac_suff_inf *acs)
{
	t_ac_rdir_inf	acrd;

	if (!init_ac_rdir(word, &acrd, is_a_cmd, 1))
	{
		ft_memdel((void**)&(acs->suff));
		return ;
	}
	autocomplete_with_infs(&acrd, acs);
	delete_ac_rdir(&acrd);
}

/*
** Alloue et retourne une string contenant la concatenation de path + / + file.
*/

static char		*build_path_to_file(const char *path, const char *file)
{
	char	*path_to_file;

	if ((path_to_file = (char*)malloc(sizeof(char) * (ft_strlen(path) +
						ft_strlen(file) + 2))) == NULL)
		return (NULL);
	path_to_file[0] = '\0';
	ft_strcat(path_to_file, path);
	ft_strcat(path_to_file, "/");
	ft_strcat(path_to_file, file);
	return (path_to_file);
}

/*
** Remplie le t_ac_suff_inf avec les informations pour autocompleter une
** commande builtin.
*/

static void		check_for_builtin_ac(t_ac_rdir_inf *acrd, t_ac_suff_inf *acs,
		t_builtin **builtin_tab)
{
	while (*builtin_tab != NULL)
	{
		acrd->cur_file_name = (*builtin_tab)->name;
		if (ft_strnequ(acrd->cur_file_name, acrd->file_word,
					acrd->file_word_len))
		{
			if (acs->suff_len == -1 || !ft_strnequ(acrd->cur_file_name +
						acrd->file_word_len, acs->suff, acs->suff_len))
			{
				build_ac_suff(acrd, acs, 1);
			}
		}
		++builtin_tab;
	}
}

/*
** Remplie le t_ac_suff_inf avec les informations pour autocompleter une
** commande builtin ou se trouvant dans le path.
*/

static void		autocomplet_cmd(const char *word, char **path_tab,
		t_builtin **builtin_tab, t_ac_suff_inf *acs)
{
	t_ac_rdir_inf	acrd;
	char			*real_word;

	check_for_builtin_ac(&acrd, acs, builtin_tab);
	while (*path_tab != NULL)
	{
		if ((real_word = build_path_to_file(*path_tab, word)) == NULL
				|| !init_ac_rdir(real_word, &acrd, 1, 1))
		{
			free(real_word);
			ft_memdel((void**)&(acs->suff));
			return ;
		}
		autocomplete_with_infs(&acrd, acs);
		free(real_word);
		delete_ac_rdir(&acrd);
		++path_tab;
	}
}

/*
** Initialise le t_ac_suff_inf passe en parametre avec les valeurs par defaut.
** Retourne 1 si l'initialisation reussi, 0 si elle rate.
*/

static int		init_ac_suff_inf(t_ac_suff_inf *acs)
{
	acs->choices = NULL;
	acs->suff_type = ACS_TYPE_NOTHING;
	acs->suff_len = -1;
	if ((acs->suff = ft_strdup("")) == NULL)
		return (0);
	return (1);
}

void			delete_ac_suff_inf(t_ac_suff_inf *acs)
{
	if (acs != NULL)
	{
		ft_lstfree(&(acs->choices));
		free(acs->suff);
		free(acs);
	}
}

t_ac_suff_inf	*autocomplet_word(const char *word, int is_a_cmd,
		const char *path, t_builtin **builtin_tab)
{
	t_ac_suff_inf	*acs;
	char			*path_cpy;
	char			**path_tab;

	if ((acs = (t_ac_suff_inf*)malloc(sizeof(t_ac_suff_inf))) == NULL)
		return (NULL);
	if (init_ac_suff_inf(acs))
	{
		if (!is_a_cmd || ft_strchr(word, '/') != NULL)
			autocomplet_from_wordpath(word, is_a_cmd, acs);
		else
		{
			if ((path_cpy = ft_strdup(path)) == NULL)
			{
				delete_ac_suff_inf(acs);
				return (NULL);
			}
			path_tab = convert_path_to_tab(path_cpy);
			if (path_tab != NULL)
				autocomplet_cmd(word, path_tab, builtin_tab, acs);
			free(path_tab);
			free(path_cpy);
		}
		if (acs->suff != NULL)
			return (acs);
	}
	delete_ac_suff_inf(acs);
	return (NULL);
}
