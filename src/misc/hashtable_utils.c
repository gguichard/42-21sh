/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/28 15:59:54 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/28 16:05:46 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "hashtable.h"

void			def_del_hashentry_val_fun(void *value, size_t value_size)
{
	(void)value_size;
	free(value);
}

size_t			def_hash_fun(const char *str)
{
	size_t			hash;
	unsigned int	c;

	hash = 5381;
	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;
	return (hash);
}

t_hashtable		*make_def_hashtable(void)
{
	t_hashtable		*new_hashtable;

	if ((new_hashtable = (t_hashtable*)(malloc(sizeof(t_hashtable)))) == NULL)
		return (NULL);
	new_hashtable->bucket_count = DEF_BUCKET_COUNT;
	new_hashtable->buckets = (t_list**)ft_memalloc(sizeof(t_list*)
			* new_hashtable->bucket_count);
	if (new_hashtable->buckets == NULL)
	{
		free(new_hashtable);
		return (NULL);
	}
	new_hashtable->hash_fun = def_hash_fun;
	new_hashtable->del_hashentry_val_fun = def_del_hashentry_val_fun;
	return (new_hashtable);
}

void			delete_hashtable(t_hashtable *hashtable)
{
	size_t	bucket_idx;
	t_list	*cur_bucket;
	t_list	*old_bucket;

	bucket_idx = 0;
	while (bucket_idx < hashtable->bucket_count)
	{
		cur_bucket = hashtable->buckets[bucket_idx];
		while (cur_bucket != NULL)
		{
			delete_hashentry(hashtable, (t_hashentry*)cur_bucket->content);
			old_bucket = cur_bucket;
			cur_bucket = cur_bucket->next;
			free(old_bucket);
		}
		++bucket_idx;
	}
	free(hashtable->buckets);
	free(hashtable);
}

int				delete_hashentry(t_hashtable *hashtable, t_hashentry *entry)
{
	if (entry != NULL)
	{
		free(entry->key);
		hashtable->del_hashentry_val_fun(entry->value, entry->value_size);
		free(entry);
	}
	return (0);
}
