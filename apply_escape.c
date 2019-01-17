#include "libft.h"
#include "str_cmd_inf.h"
#include "apply_escape.h"

char	*apply_escape(const char *str)
{
	t_str_cmd_inf	str_cmd_inf;
	char			*new_str;
	size_t			str_len;
	size_t			cur_move;

	str_len = ft_strlen(str);
	if ((new_str = ft_strndup(str, str_len)) == NULL)
		return (NULL);
	scmd_init(&str_cmd_inf, new_str);
	while (str_cmd_inf.str[str_cmd_inf.pos] != '\0')
	{
		if (str_cmd_inf.str[str_cmd_inf.pos] == '\\'
				&& (scmd_cur_char_is_in_nothing(&str_cmd_inf)
					|| (str_cmd_inf.is_in_doublequote
						&& (str_cmd_inf.str[str_cmd_inf.pos + 1] == '$'
							|| str_cmd_inf.str[str_cmd_inf.pos + 1] == '\"'
							|| str_cmd_inf.str[str_cmd_inf.pos + 1] == '\\'
							|| str_cmd_inf.str[str_cmd_inf.pos + 1] == '\n'))))
		{
			cur_move = (str_cmd_inf.str[str_cmd_inf.pos + 1] == '\n' ? 2 : 1);
			ft_memmove(new_str + str_cmd_inf.pos,
					new_str + str_cmd_inf.pos + cur_move,
					str_len + 1 - str_cmd_inf.pos - cur_move);
			str_len -= cur_move;
			if (str_cmd_inf.str[str_cmd_inf.pos] != '\0')
				str_cmd_inf.pos += (cur_move == 2 ? 0 : 1);
		}
		scmd_move_to_next_char(&str_cmd_inf);
	}
	return (new_str);
}
