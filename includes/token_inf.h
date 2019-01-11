/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_inf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 14:15:47 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/11 14:23:21 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_INF_H
# define TOKEN_INF_H

typedef enum		e_token_type
{
	TK_WORD,
	TK_OPE,
	TK_NUM_OPT
}					t_token_type;

typedef struct		s_token_inf
{
	char			*token;
	t_token_type	type;
}					t_token_inf;

#endif