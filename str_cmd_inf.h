/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_cmd_inf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 11:48:18 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/09 15:05:11 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STR_CMD_INF_H
# define STR_CMD_INF_H

#include <stdlib.h>

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

void	scmd_init(t_str_cmd_inf *str_cmd_inf, char *str);

int		scmd_cur_char_is_in_nothing(t_str_cmd_inf *str_cmd_inf);

int		scmd_cur_char_is_escaped(t_str_cmd_inf *str_cmd_inf);

int		scmd_move_to_next_char(t_str_cmd_inf *str_cmd_inf);

#endif
