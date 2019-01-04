/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_table.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 11:14:45 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/04 13:57:21 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "hash_table.h"

size_t				def_hash_fun(const char *str)
{
	size_t			hash;
	unsigned int	c;

	hash = 5381;
	while ((c = *str++))
	{
		hash = ((hash << 5) + hash) + c;
		++str;
	}
	return (hash);
}

t_hashtable			*make_def_hashtable(void)
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
	return (new_hashtable);
}

/*
** Retourne le hashentry ayant pour clef key et pour hash hash, ou NULL
** s'il n'existe pas. Cette fonction a pour but d'eviter de recalculer le hash.
*/

static t_hashentry	*get_hashentry_with_hash(t_hashtable *hashtable,
		const char *key, size_t hash)
{
	t_list	*bucket;

	bucket = hashtable->buckets[hash % hashtable->bucket_count];
	while (bucket != NULL)
	{
		if (ft_strequ(((t_hashentry*)bucket->content)->key, key))
			return ((t_hashentry*)bucket->content);
		bucket = bucket->next;
	}
	return (NULL);
}

t_hashentry			*get_hashentry(t_hashtable *hashtable, const char *key)
{
	return (get_hashentry_with_hash(hashtable, key, hashtable->hash_fun(key)));
}

/*
** Malloc et retourne un nouveau hashentry avec une copie de key et de value.
** Retourne NULL en cas d'erreur.
*/

static t_hashentry	*make_hashentry(const char *key, const char *value)
{
	t_hashentry		*new_entry;

	if ((new_entry = (t_hashentry*)malloc(sizeof(t_hashentry))) == NULL)
		return (NULL);
	if ((new_entry->key = ft_strdup(key)) == NULL
			|| (new_entry->value = ft_strdup(value)) == NULL)
	{
		free(new_entry->key);
		free(new_entry);
		return (NULL);
	}
	return (new_entry);
}

/*
** Free le contenu de la hashentry ainsi que la hashentry elle-meme (le tout si
** elle est != NULL). Retourne toujours 0.
*/

static int			delete_hashentry(t_hashentry *entry)
{
	if (entry != NULL)
	{
		free(entry->key);
		free(entry->value);
		free(entry);
	}
	return (0);
}

int					set_hashentry(t_hashtable *hashtable, const char *key,
		const char *value, int replace_if_exist)
{
	size_t			hash;
	t_hashentry		*entry;
	char			*new_value;
	t_list			*new_bucket_el;

	hash = hashtable->hash_fun(key);
	entry = get_hashentry_with_hash(hashtable, key, hash);
	if (entry != NULL && replace_if_exist)
	{
		if ((new_value = ft_strdup(value)) == NULL)
			return (0);
		free(entry->value);
		entry->value = new_value;
	}
	else if (entry == NULL)
	{
		if ((entry = make_hashentry(key, value)) == NULL ||
				(new_bucket_el = ft_lstnew(entry, sizeof(t_hashentry))) == NULL)
			return (delete_hashentry(entry));
		ft_lstadd(hashtable->buckets + (hash % hashtable->bucket_count),
				new_bucket_el);
		free(entry);
	}
	return (1);
}

void				delete_hashtable(t_hashtable *hashtable)
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
			delete_hashentry((t_hashentry*)cur_bucket->content);
			old_bucket = cur_bucket;
			cur_bucket = cur_bucket->next;
			free(old_bucket);
		}
		++bucket_idx;
	}
	free(hashtable->buckets);
	free(hashtable);
}
