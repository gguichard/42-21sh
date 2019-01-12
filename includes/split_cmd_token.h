/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd_token.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 13:18:28 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/11 16:14:35 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPLIT_CMD_TOKEN_H
# define SPLIT_CMD_TOKEN_H

# include "libft.h"
# include "str_cmd_inf.h"

/*
** Retourne la liste des tokens de la commande passee en parametre.
*/
t_list	*split_cmd_token(t_str_cmd_inf *str_cmd_inf);

#endif
