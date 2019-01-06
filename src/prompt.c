/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 17:10:37 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/06 01:14:03 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "shell.h"

void	show_prompt(t_shell *shell)
{
	write(STDOUT_FILENO, "$> ", 3);
	shell->term.offset = 3;
}
