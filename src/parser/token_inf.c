/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_inf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 14:15:47 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/16 09:42:15 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "token_inf.h"

void	del_token(void *content, size_t content_size)
{
	(void)content_size;
	free(((t_token_inf*)content)->token);
	free(content);
}
