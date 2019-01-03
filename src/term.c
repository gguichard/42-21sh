/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 19:52:57 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/03 20:59:17 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "vars.h"

int	setup_term(t_shell *shell)
{
	t_var	*var;

	var = get_var(shell->env, "TERM");
	if (var == NULL || tgetent(NULL, var->value) <= 0)
		return (0);
	tcgetattr(STDIN_FILENO, &(shell->default_term));
	ft_memcpy(&(shell->curr_term), &(shell->default_term)
		, sizeof(struct termios));
	(shell->curr_term).c_lflag &= ~(ICANON | ECHO);
	(shell->curr_term).c_cc[VMIN] = 1;
	(shell->curr_term).c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &(shell->curr_term)) < 0)
		return (0);
	return (1);
}

int	reset_term(t_shell *shell)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, &(shell->default_term)) < 0)
		return (0);
	return (1);
}
