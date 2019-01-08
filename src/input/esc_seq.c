/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   esc_seq.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 10:03:22 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/08 16:07:52 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input.h"

char	*get_valid_esc_seq(t_term *term)
{
	static char	*seqs[] = {
		ESC_SEQ_LEFT, ESC_SEQ_RIGHT,
		ESC_SEQ_UP, ESC_SEQ_DOWN,
		ESC_HOME_KEY, ESC_END_KEY,
		ESC_SHIFT_LEFT_KEY, ESC_SHIFT_RIGHT_KEY,
		ESC_SHIFT_UP_KEY, ESC_SHIFT_DOWN_KEY,
		ESC_ALT_LEFT, ESC_ALT_RIGHT,
		ESC_DEL_KEY,
		ESC_FN_F1,
		NULL
	};
	int			index;

	index = 0;
	while (seqs[index] != NULL)
	{
		if (ft_strnequ(term->seq, seqs[index], term->seq_off))
			return (seqs[index]);
		index++;
	}
	return (NULL);
}

int		append_esc_seq(t_term *term, char key)
{
	char	*seq;
	int		is_valid;

	(term->seq)[term->seq_off] = key;
	(term->seq_off)++;
	seq = get_valid_esc_seq(term);
	is_valid = seq != NULL && seq[term->seq_off] == '\0';
	if (is_valid)
		handle_esc_seq(term, seq);
	if (seq == NULL || is_valid || term->seq_off == MAX_ESC_SEQ_BYTES)
	{
		term->esc_seq = 0;
		term->seq_off = 0;
		return (is_valid ? 1 : -1);
	}
	return (0);
}

int		handle_esc_key(t_term *term, char key)
{
	if (key == 27)
		term->esc_seq = 1;
	if (!(term->esc_seq))
		return (0);
	return (append_esc_seq(term, key) >= 0);
}

void	handle_esc_seq(t_term *term, const char *seq)
{
	if (ft_strequ(seq, ESC_SEQ_LEFT))
		move_cursor_left(term);
	else if (ft_strequ(seq, ESC_SEQ_RIGHT))
		move_cursor_right(term);
	else if (ft_strequ(seq, ESC_DEL_KEY))
		handle_del_key(term);
	else if (ft_strequ(seq, ESC_HOME_KEY))
		move_cursor_home(term);
	else if (ft_strequ(seq, ESC_END_KEY))
		move_cursor_end(term);
	else if (ft_strequ(seq, ESC_SHIFT_LEFT_KEY))
		move_cursor_prev_word(term);
	else if (ft_strequ(seq, ESC_SHIFT_RIGHT_KEY))
		move_cursor_next_word(term);
	else if (ft_strequ(seq, ESC_SHIFT_UP_KEY))
		move_cursor_up(term);
	else if (ft_strequ(seq, ESC_SHIFT_DOWN_KEY))
		move_cursor_down(term);
	else if (ft_strequ(seq, ESC_FN_F1))
	{
		term->visual_mode = !(term->visual_mode);
		if (term->visual_mode)
		{
			term->cursor = ft_min(term->size - 1, term->cursor);
			term->select.begin = term->cursor;
			term->select.end = term->cursor;
		}
		refresh_prompt_command(term);
		return ;
	}
	if (term->visual_mode)
	{
		term->select.end = term->cursor;
		refresh_prompt_command(term);
	}
}
