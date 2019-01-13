/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 15:09:50 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/13 14:06:10 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "libft.h"
# include "token_inf.h"

/*
** SHELL GRAMMAR parsing.
*/
void	next_token(t_list **curr);
int		accept_token(t_list **curr, t_token_type type);
int		expect_token(t_list **curr, t_token_type type);
int		parse_operator(t_list **curr);
void	parse_commands(t_list *tokens);

/*
** COMMAND EXECUTION.
*/
t_list	*get_commands_list(t_list *tokens);

#endif
