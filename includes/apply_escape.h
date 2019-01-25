/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_escape.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 08:58:37 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/25 15:29:39 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APPLY_ESCAPE_H
# define APPLY_ESCAPE_H

/*
** Alloue et retourne une nouvelle chaine de caractere avec les caracteres
** echappes remplaces par les caracteres adequats. Retourne NULL en
** cas d'erreur.
*/
char	*apply_escape(const char *str);

/*
** Alloue et retourne une nouvelle chaine de caractere avec les newlines
** echappes (non presents dans des quotes) supprimes. Si apply_on_last_char
** vaut faux et que le dernier caractere est un newline l'echappement ne sera
** pas applique. Retourne NULL en cas d'erreur.
*/
char	*apply_only_newline_escape(const char *str, int apply_on_last_char);

#endif
