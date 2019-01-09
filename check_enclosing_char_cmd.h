/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_enclosing_char_cmd.h                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 09:12:50 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/09 09:58:11 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHECK_ENCLOSING_CHAR_CMD_H
# define CHECK_ENCLOSING_CHAR_CMD_H

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

t_str_cmd_err	check_enclosing_char_cmd(t_str_cmd_inf *str_cmd_inf);

#endif
