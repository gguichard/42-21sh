/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_inf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 14:15:47 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/16 09:42:15 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_INF_H
# define TOKEN_INF_H

typedef enum		e_token_type
{
	TK_NOTHING,
	TK_WORD,
	TK_OPE,
	TK_NUM_OPT,
	TK_STR_OPT,
	TK_CMD_SEP
}					t_token_type;

typedef struct		s_token_inf
{
	char			*token;
	t_token_type	type;
}					t_token_inf;

#endif
