/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   esc_seq.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 10:03:22 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/04 11:29:17 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

char	*get_valid_esc_seq(t_shell *shell)
{
	static char	*seqs[] = {
		ESC_SEQ_LEFT,
		ESC_SEQ_RIGHT,
		ESC_SEQ_UP,
		ESC_SEQ_DOWN,
		ESC_DEL_KEY,
		NULL
	};
	int			index;

	index = 0;
	while (seqs[index] != NULL)
	{
		if (ft_strnequ(shell->term.seq, seqs[index], shell->term.seq_off))
			return (seqs[index]);
		index++;
	}
	return (NULL);
}

int		append_esc_seq(t_shell *shell, char key)
{
	char	*seq;
	int		is_valid;

	(shell->term.seq)[shell->term.seq_off] = key;
	(shell->term.seq_off)++;
	seq = get_valid_esc_seq(shell);
	is_valid = seq != NULL && seq[shell->term.seq_off] == '\0';
	if (is_valid)
		handle_esc_key(shell, seq);
	if (seq == NULL || is_valid || shell->term.seq_off == MAX_ESC_SEQ_BYTES)
	{
		shell->term.esc_seq = 0;
		shell->term.seq_off = 0;
		return (is_valid ? 1 : -1);
	}
	return (0);
}
