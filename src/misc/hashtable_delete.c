/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_delete.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 16:19:31 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/29 16:24:30 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "hashtable.h"

int		delete_all_hashentry(t_hashtable *hashtable)
{
	size_t	bucket_idx;
	t_list	*cur_bucket;

	bucket_idx = 0;
	while (bucket_idx < hashtable->bucket_count)
	{
		cur_bucket = hashtable->buckets[bucket_idx];
		while (cur_bucket != NULL)
		{
			delete_hashentry(hashtable, (t_hashentry*)cur_bucket->content);
			cur_bucket = cur_bucket->next;
		}
		++bucket_idx;
	}
	return (0);
}
