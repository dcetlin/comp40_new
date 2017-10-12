#include "bitpack.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
	printf("[%016lx]\n", Bitpack_news(0, 40, 0, -1));
	(void) argv;
	(void) argc;
	return 0;
}