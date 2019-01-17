/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 15:09:50 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/17 13:45:27 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "libft.h"
# include "token_inf.h"

void	next_token(t_list **curr);
int		accept_token(t_list **curr, t_token_type type);
int		expect_token(t_list **curr, t_token_type type);
int		parse_operator(t_list **curr);
int		parse_commands(t_list *tokens);

#endif
