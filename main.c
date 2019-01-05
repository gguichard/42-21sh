/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 12:46:28 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/05 14:16:52 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "autocomplete.h"

int		main(int argc, char **argv)
{
	char	*ac;
	int		idx;

	for (int idx = 1; idx < argc; ++idx)
	{
		ac = autocomplet_word(argv[idx], 0, "");
		if (ac != NULL)
		{
			ft_putstr(argv[idx]);
			ft_putstr(" => ");
			ft_putendl(ac);
		}
		free(ac);
	}
	return (0);
}
