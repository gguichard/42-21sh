/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_inf.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 16:22:42 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/18 16:40:38 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "redirect_inf.h"

void	del_redirect(void *content, size_t content_size)
{
	(void)content_size;
	free(((t_redirect_inf*)content)->to_word);
	free(content);
}
