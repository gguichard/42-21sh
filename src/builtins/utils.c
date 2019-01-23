/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 00:24:00 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/24 00:54:51 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	set_dir_to_canonical_form(char *dir)
{
	char	*start;
	char	*slash;

	if (dir[0] == '\0')
		return ;
	start = dir;
	while ((slash = ft_strchr(start, '/')) != NULL)
	{
		if (ft_strnequ(".", start, (size_t)(slash - start)))
		{
			ft_strcpy(start, slash + 1);
			continue ;
		}
		start = slash + 1;
	}
}
