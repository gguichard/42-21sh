/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_cmd_inf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 11:48:18 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/10 10:28:31 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STR_CMD_INF_H
# define STR_CMD_INF_H

# include <stdlib.h>

typedef enum	e_str_cmd_err
{
	SCMDERR_NOERROR = 0,
	SCMDERR_ENDTOOSOON,
	SCMDERR_BADCHAR
}				t_str_cmd_err;

typedef struct	s_str_cmd_inf
{
	int			is_in_quote;
	int			is_in_doublequote;
	int			is_in_var_bracket;
	size_t		pos;
	char		*str;
}				t_str_cmd_inf;

/*
** Initialise le str_cmd_inf en mettant les valeurs a 0.
*/
void			scmd_init(t_str_cmd_inf *str_cmd_inf, char *str);

/*
** Retourne 1 si le curseur a la position actuelle n'est dans aucun enclosing
** char (",',{), 0 sinon.
*/
int				scmd_cur_char_is_in_nothing(t_str_cmd_inf *str_cmd_inf);

/*
** Retourne 1 si le curseur a la position actuelle est echappe, 0 sinon.
*/
int				scmd_cur_char_is_escaped(t_str_cmd_inf *str_cmd_inf);

/*
** Deplace le curseur vers la droite en modifiant si necessaire l'etat de
** str_cmd_inf (dans une quote etc).
*/
int				scmd_move_to_next_char(t_str_cmd_inf *str_cmd_inf);

#endif
