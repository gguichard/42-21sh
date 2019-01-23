/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete_cmdline.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 14:17:37 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/23 08:50:30 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "split_cmd_token.h"
#include "apply_escape.h"
#include "str_cmd_inf.h"
#include "token_inf.h"
#include "shell.h"
#include "autocomplete.h"

t_ac_suff_inf	*autocomplete_cmdline(t_str_cmd_inf *scmd, const char *path,
		t_builtin **builtin_tab)
{
	t_list			*all_tokens;
	t_list			*cur_token;
	const char		*last_word;
	char			*real_word;
	int				word_is_cmd;
	t_ac_suff_inf	*result;

	all_tokens = split_cmd_token(scmd);
	last_word = "";
	word_is_cmd = -1;
	cur_token = all_tokens;
	while (cur_token != NULL)
	{
		last_word = "";
		if (get_tk(cur_token)->type == TK_WORD)
		{
			last_word = get_tk(cur_token)->token;
			word_is_cmd = (word_is_cmd == -1 ? 1 : 0);
		}
		else if (get_tk(cur_token)->type == TK_CMD_SEP
				|| (get_tk(cur_token)->type == TK_OPE
					&& ft_strequ(get_tk(cur_token)->token, "|")))
		{
			word_is_cmd = -1;
		}
		else
			word_is_cmd = 0;
		cur_token = cur_token->next;
	}
	if (last_word[0] != '\0' && scmd_cur_char_is_in_nothing(scmd)
			&& (scmd->str[scmd->pos - 1] == ' '
				|| scmd->str[scmd->pos - 1] == '\t'))
	{
		last_word = "";
		word_is_cmd = (word_is_cmd == -1 ? 1 : 0);
	}
	if ((real_word = apply_escape(last_word)) == NULL)
	{
		ft_lstdel(&all_tokens, del_token);
		return (NULL);
	}
	result = autocomplete_word(real_word, (word_is_cmd == 0 ? 0 : 1), path,
			builtin_tab);
	free(real_word);
	ft_lstdel(&all_tokens, del_token);
	return (result);
}
