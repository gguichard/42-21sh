/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 17:10:37 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/18 09:39:15 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "shell.h"

void	show_prompt(t_shell *shell)
{
	char	*prompt;

	if (shell->term.multiline != NULL)
		prompt = (shell->term.visual_mode) ? "(visual) > " : "> ";
	else
		prompt = (shell->term.visual_mode) ? "(visual) $> " : "$> ";
	shell->term.offset = write(STDOUT_FILENO, prompt, ft_strlen(prompt));
}
