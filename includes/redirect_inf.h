/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_inf.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 10:20:19 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/21 08:29:32 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_INF_H
# define REDIRECT_INF_H

# define FD_ERROR		-1
# define FD_DEFAULT		-2
# define FD_AMPERSAND	-3
# define FD_NOTSET		-4

typedef enum			e_redirect_type
{
	RD_L,
	RD_R,
	RD_LR,
	RD_LL,
	RD_RR,
	RD_ERROR
}						t_redirect_type;

typedef struct			s_redirect_inf
{
	int					from_fd;
	t_redirect_type		red_type;
	int					to_fd;
	int					close_to_fd;
	char				*to_word;
}						t_redirect_inf;

void					del_redirect(void *content, size_t content_size);

t_redirect_type			redirection_str_to_type(const char *str);

#endif