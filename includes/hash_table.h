/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_table.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 10:58:47 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/04 13:40:58 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASH_TABLE_H
# define HASH_TABLE_H

# include "libft.h"

# define DEF_BUCKET_COUNT 2053

typedef struct	s_hashentry
{
	char		*key;
	char		*value;
}				t_hashentry;

typedef struct	s_hashtable
{
	t_list		**buckets;
	size_t		bucket_count;
	size_t		(*hash_fun)(const char *str);
}				t_hashtable;

size_t			def_hash_fun(const char *str);

t_hashtable		*make_def_hashtable(void);

t_hashentry		*get_hashentry(t_hashtable *hashtable, const char *key);

int				set_hashentry(t_hashtable *hashtable, const char *key,
		const char *value, int replace_if_exist);

void			delete_hashtable(t_hashtable *hashtable);

#endif
