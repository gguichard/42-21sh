/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_table.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 10:58:47 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/04 13:52:56 by fwerner          ###   ########.fr       */
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

/*
** Fonction de hashage d'une string par defaut.
*/
size_t			def_hash_fun(const char *str);

/*
** Retourne une hashtable initilialisee avec les parametres par defaut, ou NULL
** si erreur.
*/
t_hashtable		*make_def_hashtable(void);

/*
** Retourne le hashentry ayant pour clef key, ou NULL s'il n'existe pas.
*/
t_hashentry		*get_hashentry(t_hashtable *hashtable, const char *key);

/*
** Set l'entree de la hashtable ayant pour clef key a la valeur value. Si
** l'entree existe deja et que replace_if_exist vaut 0 ne fait rien d'autre
** que retourner 1. Retourn 0 en cas d'erreur et 1 en cas de succes.
*/
int				set_hashentry(t_hashtable *hashtable, const char *key,
		const char *value, int replace_if_exist);

/*
** Free l'integralite des elements de la hashtable ainsi que la hashtable
** elle-meme.
*/
void			delete_hashtable(t_hashtable *hashtable);

#endif
