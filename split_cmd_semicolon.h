/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd_semicolon.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 13:19:19 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/10 10:31:19 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPLIT_CMD_SEMICOLON_H
# define SPLIT_CMD_SEMICOLON_H

# include "libft.h"
# include "str_cmd_inf.h"

/*
** Retourne la liste des sous commandes de la commande passe en parametres
** (celles qui sont separees par ';').
*/
t_list	*split_cmd_semicolon(t_str_cmd_inf *str_cmd_inf);

#endif
