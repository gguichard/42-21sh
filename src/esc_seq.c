/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   esc_seq.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 10:03:22 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/04 10:03:56 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

int	is_valid_esc_seq(t_shell *shell)
{
	static char	*seqs[] = {
		ESC_SEQ_LEFT,
		ESC_SEQ_RIGHT,
		ESC_SEQ_UP,
		ESC_SEQ_DOWN,
		NULL
	};
	int			index;

	index = 0;
	while (seqs[index] != NULL)
	{
		if (ft_strnequ(shell->term.seq, seqs[index], shell->term.seq_off))
			return (ft_strlen(seqs[index]) == (size_t)shell->term.seq_off);
		index++;
	}
	return (-1);
}

int	append_esc_seq(t_shell *shell, char key)
{
	int	ret;

	(shell->term.seq)[shell->term.seq_off] = key;
	(shell->term.seq_off)++;
	ret = is_valid_esc_seq(shell);
	if (ret == 1)
		ft_printf("got good esc seq\n");
	if (ret != 0 || shell->term.seq_off == MAX_ESC_SEQ_BYTES)
	{
		shell->term.esc_seq = 0;
		shell->term.seq_off = 0;
		return (ret);
	}
	return (0);
}
