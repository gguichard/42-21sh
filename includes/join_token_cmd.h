/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_token_cmd.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 14:09:00 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/21 14:35:19 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef JOIN_TOKEN_CMD_H
# define JOIN_TOKEN_CMD_H

# include "libft.h"

/*
** Alloue et retourne une liste de commande representant la liste des tokens
** passee en parametre. Retourne NULL en cas d'erreur ou de liste vide.
*/
t_list	*join_token_cmd(t_list *token_lst);

/*
** Remplie le cmd_inf avec les infos de la prochaine commande dans token_lst,
** deplace aussi la tete de liste. Retourne 1 en cas de succes, 0 en cas
** d'echec.
*/
int		set_cur_cmd(t_cmd_inf *cmd_inf, t_list **token_lst);

#endif
