#include "bitpack.h"
#include <stdio.h>

int main(int argc, char const *argv[])
{
	int64_t x = Bitpack_news(-1, 0, 0, 0);
	printf("[%016lx] ==> (%lu)\n", x , x); //TODO not enough f's
	(void) argv;
	(void) argc;
	return 0;
}