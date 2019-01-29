/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 19:52:57 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/30 00:49:42 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/ioctl.h>
#include "shell.h"
#include "vars.h"
#include "input.h"

void	setup_term(t_shell *shell)
{
	t_var	*var;

	if (shell->term.legacy_mode)
		return ;
	var = get_var(shell->env, "TERM");
	if (var == NULL || tgetent(NULL, var->value) <= 0)
	{
		shell->term.legacy_mode = 1;
		return ;
	}
	tcgetattr(STDIN_FILENO, &(shell->term.default_term));
	ft_memcpy(&(shell->term.curr_term), &(shell->term.default_term)
		, sizeof(struct termios));
	(shell->term.curr_term).c_lflag &= ~(ICANON | ECHO);
	(shell->term.curr_term).c_cc[VMIN] = 1;
	(shell->term.curr_term).c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &(shell->term.curr_term)) < 0)
		shell->term.legacy_mode = 1;
}

void	reset_term(t_shell *shell)
{
	if (!(shell->term.legacy_mode))
		tcsetattr(STDIN_FILENO, TCSANOW, &(shell->term.default_term));
}

void	update_winsize(t_term *term)
{
	int	ret;

	if (term->legacy_mode)
		return ;
	ret = ioctl(STDOUT_FILENO, TIOCGWINSZ, &(term->winsize));
	if (ret == -1)
		term->legacy_mode = 1;
	else
	{
		if (term->winsize.ws_col <= 0)
			term->winsize.ws_col = 1;
		update_pos_data(term);
	}
}
