
#    Updated: 2019/01/18 14:32:28 by gguichar         ###   ########.fr        #
			misc/error.c misc/check_path.c misc/hash_table.c \
			misc/convert_path_to_tab.c misc/utils.c \
			misc/check_enclosing_char_cmd.c
clean:
	$(MAKE) -C libft clean
	/bin/rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C libft fclean
	/bin/rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
