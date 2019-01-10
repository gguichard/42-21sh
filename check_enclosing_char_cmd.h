/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_enclosing_char_cmd.h                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 09:12:50 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/10 10:30:04 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECK_ENCLOSING_CHAR_CMD_H
# define CHECK_ENCLOSING_CHAR_CMD_H

# include <stdlib.h>
# include "str_cmd_inf.h"

/*
** Retourne SCMDERR_NOERROR si la commande str_cmd_inf a des enclosing char
** valides ou l'erreur correspondante sinon.
*/
t_str_cmd_err	check_enclosing_char_cmd(t_str_cmd_inf *str_cmd_inf);

#endif
