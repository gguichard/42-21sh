/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_escape.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 08:58:37 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/18 08:58:39 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef APPLY_ESCAPE_H
# define APPLY_ESCAPE_H

/*
** Alloue et retourne une nouvelle chaine de caractere avec les caracteres
** echappes remplaces par les caracteres adequats.
*/
char	*apply_escape(const char *str);

#endif
