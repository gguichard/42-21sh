#include <stdio.h>
#include "get_next_line.h"
#include "apply_escape.h"

int		main(int argc, char **argv)
{
	char *line;

	get_next_line(0, &line);
	printf("%s\n", apply_escape(line));
	return (0);
}
