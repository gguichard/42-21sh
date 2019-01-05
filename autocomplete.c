/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 09:10:18 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/05 12:46:14 by fwerner          ###   ########.fr       */
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

static char		*autocomplet_from_wordpath(const char *word, int is_a_cmd)
{
	DIR				*dir;
	struct dirent	*dirent;
	char			*last_slash;
	char			*dir_to_use;
	char			*file_word;
	char			*cur_file_path;
	char			*ac_suff;
	size_t			ac_suff_len;
	size_t			file_word_len;
	struct stat		stat_buf;

	if ((last_slash = ft_strrchr(word, '/')) == NULL)
		dir_to_use = ft_strdup("./");
	else
		dir_to_use = ft_strndup(word, last_slash - word + 1);
	if (dir_to_use == NULL || (file_word = ft_strdup(
					last_slash == NULL ? word : last_slash + 1)) == NULL)
	{
		free(dir_to_use);
		return (NULL);
	}
	file_word_len = ft_strlen(file_word);
	if ((dir = opendir(dir_to_use)) == NULL)
	{
		free(file_word);
		free(dir_to_use);
		return (ft_strdup(""));
	}
	ac_suff_len = -1;
	if ((ac_suff = ft_strdup("")) == NULL)
	{
		closedir(dir);
		free(file_word);
		free(dir_to_use);
		return (NULL);
	}
	while ((dirent = readdir(dir)) != NULL)
	{
		if ((cur_file_path = ft_strjoin(dir_to_use, dirent->d_name)) == NULL)
		{
			ac_suff = NULL;
			break ;
		}
		if (stat(cur_file_path, &stat_buf) == -1)
		{
			free(cur_file_path);
			continue ;
		}
		if ((dirent->d_name[0] != '.' || file_word[0] == '.') &&
				(!is_a_cmd || S_ISDIR(stat_buf.st_mode) ||
					(S_ISREG(stat_buf.st_mode) && access(cur_file_path, X_OK))))
		{
			if (ft_strnequ(dirent->d_name, file_word, file_word_len)
					&& (ac_suff_len == -1 || !ft_strnequ(dirent->d_name + file_word_len, ac_suff, ac_suff_len)))
			{
				if (ac_suff_len == -1)
				{
					free(ac_suff);
					if ((ac_suff = ft_strdup(dirent->d_name + file_word_len)) == NULL)
						ac_suff_len = 0;
					else
						ac_suff_len = ft_strlen(dirent->d_name + file_word_len);
				}
				else
				{
					ac_suff_len =  count_same_char(dirent->d_name + file_word_len, ac_suff);
					ac_suff[ac_suff_len] = '\0';
				}
				if (ac_suff_len == 0)
				{
					free(cur_file_path);
					break ;
				}
			}
		}
		free(cur_file_path);
	}
	closedir(dir);
	free(file_word);
	free(dir_to_use);
	return (ac_suff);
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
