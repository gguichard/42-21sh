#include <stdio.h>
#include "apply_escape.h"

int		main(int argc, char **argv)
{
	printf("%s\n", apply_escape(argv[1]));
	return (0);
}
