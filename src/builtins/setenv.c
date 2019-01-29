/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 15:00:41 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/29 16:32:00 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "vars.h"
#include "hashtable.h"
#include "builtins.h"

static int	is_valid_identifier(const char *key)
{
	size_t	offset;

	if (ft_isdigit(key[0]))
		return (0);
	offset = 0;
	while (key[offset] != '\0')
	{
		if (!ft_isalnum(key[offset]) && key[offset] != '_')
			return (0);
		offset++;
	}
	return (1);
}

static int	update_env_var(t_shell *shell, const char *key, const char *value)
{
	if (!is_valid_identifier(key))
		return (0);
	if (ft_strequ("PATH", key))
		delete_hashentries(shell->exec_hashtable);
	update_var(&(shell->env), key, value);
	return (1);
}

int			builtin_setenv(t_shell *shell, int argc, char **argv)
{
	int		index;
	char	*tmp;
	int		ret;

	index = 1;
	ret = 0;
	while (index < argc)
	{
		tmp = ft_strchr(argv[index], '=');
		if (tmp != NULL)
		{
			*tmp = '\0';
			if (!update_env_var(shell, argv[index], tmp + 1))
			{
				ret = 1;
				ft_dprintf(2, "%s: %s: `%s': Not a valid identifier\n"
						, ERR_PREFIX, argv[0], argv[index]);
			}
		}
		index++;
	}
	return (ret);
}
