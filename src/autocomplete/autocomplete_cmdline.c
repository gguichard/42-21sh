/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete_cmdline.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 14:17:37 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/28 10:32:56 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "split_cmd_token.h"
#include "expand_vars.h"
#include "apply_escape.h"
#include "vars.h"
#include "str_cmd_inf.h"
#include "token_inf.h"
#include "shell.h"
#include "autocomplete.h"

static void				process_token(t_token_inf *tk_inf,
		const char **last_word, int *word_is_cmd)
{
	*last_word = "";
	if (tk_inf->type == TK_WORD)
	{
		*last_word = tk_inf->token;
		*word_is_cmd = (*word_is_cmd == -1 ? 1 : 0);
	}
	else if (tk_inf->type == TK_CMD_SEP
			|| (tk_inf->type == TK_OPE
				&& ft_strequ(tk_inf->token, "|")))
		*word_is_cmd = -1;
	else
		*word_is_cmd = 0;
}

static t_ac_suff_inf	*call_ac_word(const char *last_word, int word_is_cmd,
		t_shell *shell, t_builtin **builtins)
{
	char			*home_expanded_word;
	char			*real_word;
	t_ac_suff_inf	*result;

	if ((home_expanded_word = expand_home(last_word, shell, 1)) == NULL)
		return (NULL);
	if ((real_word = apply_escape(home_expanded_word)) == NULL)
	{
		free(home_expanded_word);
		return (NULL);
	}
	free(home_expanded_word);
	result = autocomplete_word(shell, real_word, (word_is_cmd == 0 ? 0 : 1)
			, builtins);
	free(real_word);
	return (result);
}

t_ac_suff_inf			*autocomplete_cmdline(t_str_cmd_inf *scmd,
		t_shell *shell, t_builtin **builtins)
{
	t_list			*all_tokens;
	t_list			*cur_token;
	const char		*last_word;
	int				word_is_cmd;
	t_ac_suff_inf	*res;

	all_tokens = split_cmd_token(scmd);
	last_word = "";
	word_is_cmd = -1;
	cur_token = all_tokens;
	while (cur_token != NULL)
	{
		process_token(get_tk(cur_token), &last_word, &word_is_cmd);
		cur_token = cur_token->next;
	}
	if (last_word[0] != '\0' && scmd_cur_char_is_in_nothing(scmd)
			&& scmd->pos > 0
			&& ft_strchr(" \t\n", scmd->str[scmd->pos - 1]) != NULL
			&& !scmd_char_at_is_escaped(scmd, scmd->pos - 1))
	{
		last_word = "";
		word_is_cmd = (word_is_cmd == -1 ? 1 : 0);
	}
	res = call_ac_word(last_word, (word_is_cmd == 0 ? 0 : 1), shell, builtins);
	ft_lstdel(&all_tokens, del_token);
	return (res);
}
