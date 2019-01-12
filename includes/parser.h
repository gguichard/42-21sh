/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/12 15:09:50 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/12 15:27:16 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "libft.h"
# include "token_inf.h"

/*
** SHELL GRAMMAR parsing.
*/
void	next_token(t_list **tokens);
int		accept_token(t_list **tokens, t_token_type type);
int		expect_token(t_list **tokens, t_token_type type);
void	parse_command(t_list **tokens);
void	split_commands(t_list *tokens);

/*
** COMMAND EXECUTION.
*/
t_list	*get_commands_list(t_list *tokens);

#endif
