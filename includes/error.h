/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 15:26:54 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/03 16:04:16 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

typedef enum	e_error
{
	ERRC_NOERROR		= 0,
	ERRC_UNEXPECTED		= 1,
	ERRC_FILENOTFOUND,
	ERRC_CMDNOTFOUND,
	ERRC_WRONGNUMOFARGS,
	ERRC_TOOMANYARGS,
	ERRC_NOTADIR,
	ERRC_ISADIR,
	ERRC_NOEXECRIGHT
}				t_error;

/*
** Free la variable var et retourne err.
** Fonction utilitaire ayant pour but de reduire le nombre de ligne.
*/
t_error		del_then_ret_err(void *var, t_error err);

#endif
