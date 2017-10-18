#include "bitpack.h"
#include <assert.h>
#include <stdio.h>
Except_T Bitpack_Overflow = { "Overflow packing bits" };

bool Bitpack_fitsu(uint64_t n, unsigned width)
{
	if (width > 64) {
		RAISE(Bitpack_Overflow);
	}

	uint64_t max = ~0;
	max >>= 64 - width;

	// if (!(n <= max && width))
	// 	fprintf(stderr, "width: %u | (%016lx = %lu) < (%016lx = %lu)\n", width, n, n, max, max);

	return (n <= max && width);
}

bool Bitpack_fitss(int64_t n, unsigned width)
{
	if (width > 64) {
		RAISE(Bitpack_Overflow);
	}

	//printf("Width: %u\n", width);

	int64_t min = ((int64_t) 1 << (63)) >> (64 - width);
	int64_t max = ((int64_t) 1 << (width - 1)) - 1;

	//printf("{{%016lx} = {%li}} <= {{%016lx} = {%li}} <= {{%016lx} = {%li}}\n",min, min, n, n, max, max);

	return (n >= min && n <= max && width);
}

uint64_t create_mask(unsigned width, unsigned lsb) //returns 0 for width = 0;
{
	uint64_t mask = ~0;
	mask = (((mask << (64 - width - lsb)) >> (64 - width)) << lsb);
	return mask;
}

uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb) //returns 0 for width = 0;
{
	if (width > 64 && width + lsb > 64) {
		RAISE(Bitpack_Overflow);
	}

	uint64_t mask = create_mask(width, lsb);

	//printf("mask: {%lx = %lu}\n", mask, mask);
	//printf("word: {%lx = %lu}\n", word, word);

	return (word & mask) >> (lsb);
}
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb) //returns 0 for width = 0;
{
	if (width > 64 && width + lsb > 64) {
		RAISE(Bitpack_Overflow);
	}

	uint64_t mask = create_mask(width, lsb);

	// printf("mask: {%lx = %li}\n", mask, mask);
	// printf("word: {%lx = %li}\n", word, word);

	// printf("(word & mask) << (64 - width - lsb) = %lx\n", (word & mask) << (64 - width - lsb));

	return ((int64_t)(word & mask) << (64 - width - lsb)) >> (64 - width);
}

uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb, uint64_t value)
{
	if ((width > 64 && width + lsb > 64) || !Bitpack_fitsu(value, width)) {
		RAISE(Bitpack_Overflow);
	}

	uint64_t mask = ~create_mask(width, lsb);

	return (word & mask) | (value << lsb);
}

uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,  int64_t value) //Bitpack_news(0, 40, 0, -1)); //TODO not enough f's
{
	if ((width > 64 && width + lsb > 64) || !Bitpack_fitss(value, width)) {
		RAISE(Bitpack_Overflow);
	}

	uint64_t mask = ~create_mask(width, lsb);

	// printf("\n ");

	// printf("\n");
	//printf("{%016lx}\n ==/>  \n{%016lx}\n", value, word);

	//printf("(value & (2 << width) - 1) = %lx & %lx\n", value, create_mask(width, 0));

	return (word & mask) | ((value & (create_mask(width, 0))) << lsb);
}