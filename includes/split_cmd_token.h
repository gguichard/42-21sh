/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd_token.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 13:18:28 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/29 11:10:06 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPLIT_CMD_TOKEN_H
# define SPLIT_CMD_TOKEN_H

# include "libft.h"
# include "token_inf.h"
# include "str_cmd_inf.h"

t_token_type	get_token_for_opt_add(t_str_cmd_inf *str_cmd_inf
		, const char *token_start, int *end_by_and);

int				get_cur_token_len(const char *token_start);

int				cur_token_is_number(t_str_cmd_inf *str_cmd_inf
		, const char *token_start);

int				is_a_sep_char(char c);

int				add_cur_token_to_lst(t_list **token_lst
		, t_str_cmd_inf *str_cmd_inf, const char *token_start
		, t_token_type token_type);

int				add_token_to_lst(t_list **token_lst, char *token_str
		, t_token_type token_type);

/*
** Retourne la liste des tokens de la commande passee en parametre.
*/
t_list			*split_cmd_token(t_str_cmd_inf *str_cmd_inf);

#endif
