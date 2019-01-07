/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 09:10:18 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/07 11:17:39 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "libft.h"
#include "convert_path_to_tab.h"
#include "autocomplete.h"

static size_t	count_same_char(const char *str1, const char *str2)
{
	int		count;

	count = 0;
	while (str1[count] != '\0' && str1[count] == str2[count])
		++count;
	return (count);
}

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
	acrd->dirent = NULL;
	acrd->cur_file_path = NULL;
	return (1);
}

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

static int		readdir_to_dirent(t_ac_rdir_inf *acrd, t_ac_suff_inf *acs)
{
	while ((acrd->dirent = readdir(acrd->dir)) != NULL)
	{
		if ((acrd->cur_file_path = ft_strjoin(acrd->dir_to_use,
						acrd->dirent->d_name)) == NULL)
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

static int		valid_file_for_ac(t_ac_rdir_inf *acrd)
{
	if (acrd->dirent->d_name[0] != '.' || acrd->file_word[0] == '.')
	{
		if (!acrd->need_to_be_cmd
				|| (acrd->can_be_dir && S_ISDIR(acrd->stat_buf.st_mode))
				|| (S_ISREG(acrd->stat_buf.st_mode)
					&& access(acrd->cur_file_path, X_OK) == 0))
		{
			if (ft_strnequ(acrd->dirent->d_name, acrd->file_word,
					acrd->file_word_len))
			{
				return (1);
			}
		}
	}
	return (0);
}

static int		build_ac_suff(t_ac_rdir_inf *acrd, t_ac_suff_inf *acs)
{
	if (acs->suff_len == -1)
	{
		free(acs->suff);
		acs->suff_len = ft_strlen(acrd->dirent->d_name + acrd->file_word_len);
		if ((acs->suff =
					(char*)malloc(sizeof(char) * (acs->suff_len + 2))) == NULL)
			acs->suff_len = 0;
		else
		{
			ft_memcpy(acs->suff, acrd->dirent->d_name + acrd->file_word_len,
					acs->suff_len + 1);
			acs->is_dir = S_ISDIR(acrd->stat_buf.st_mode);
		}
	}
	else
	{
		acs->is_dir = 0;
		acs->suff_len = count_same_char(acrd->dirent->d_name +
				acrd->file_word_len, acs->suff);
		acs->suff[acs->suff_len] = '\0';
	}
	return (!(acs->suff_len == 0 && !acs->is_dir));
}

static int		try_ac_for_this_file(t_ac_rdir_inf *acrd, t_ac_suff_inf *acs)
{
	if (valid_file_for_ac(acrd))
	{
		if (acs->suff_len == -1 || !ft_strnequ(acrd->dirent->d_name +
					acrd->file_word_len, acs->suff, acs->suff_len))
		{
			if (!build_ac_suff(acrd, acs))
			{
				ft_memdel((void**)&(acrd->cur_file_path));
				return (0);
			}
		}
		else
			acs->is_dir = 0;
	}
	ft_memdel((void**)&(acrd->cur_file_path));
	return (1);
}

static void		autocomplete_with_infs(t_ac_rdir_inf *acrd, t_ac_suff_inf *acs)
{
	if ((acrd->dir = opendir(acrd->dir_to_use)) != NULL)
	{
		while (readdir_to_dirent(acrd, acs))
			if (!try_ac_for_this_file(acrd, acs))
				break ;
	}
}

static char		*autocomplet_from_wordpath(const char *word, int is_a_cmd)
{
	t_ac_rdir_inf	acrd;
	t_ac_suff_inf	acs;

	if (!init_ac_rdir(word, &acrd, is_a_cmd, 1))
		return (NULL);
	acs.is_dir = 0;
	acs.suff_len = -1;
	if ((acs.suff = ft_strdup("")) == NULL)
	{
		delete_ac_rdir(&acrd);
		return (NULL);
	}
	autocomplete_with_infs(&acrd, &acs);
	delete_ac_rdir(&acrd);
	if (acs.is_dir && acs.suff != NULL)
		ft_memcpy(acs.suff + acs.suff_len, "/", 2);
	return (acs.suff);
}

static char		*autocomplet_cmd(const char *word, char **path_tab)
{
	(void)word;
	(void)path_tab;
	return (ft_strdup(""));
}

char			*autocomplet_word(const char *word, int is_a_cmd,
		const char *path)
{
	char	*word_suffix;
	char	*path_cpy;
	char	**path_tab;

	word_suffix = NULL;
	if (!is_a_cmd || ft_strchr(word, '/') != NULL)
		word_suffix = autocomplet_from_wordpath(word, is_a_cmd);
	else
	{
		if ((path_cpy = ft_strdup(path)) == NULL)
			return (NULL);
		path_tab = convert_path_to_tab(path_cpy);
		if (path_tab != NULL)
			word_suffix = autocomplet_cmd(word, path_tab);
		free(path_tab);
		free(path_cpy);
	}
	return (word_suffix);
}
