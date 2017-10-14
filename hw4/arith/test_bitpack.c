#include "bitpack.h"
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char const *argv[])
{
	bool x = Bitpack_fitss(1, 2);
	printf("[%s]\n", x? "TRUE": "FALSE"); //TODO not enough f's
	(void) argv;
	(void) argc;
	return 0;
}