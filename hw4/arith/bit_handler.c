#include "headers/bit_handler.h"
#include "headers/algorithms.h"
#include "headers/bitpack.h"
#include <assert.h>

void print_bits(uint32_t word)
{
	for (int k = 32; k >= 0; k--) {
		putchar(Bitpack_getu(word, 1, k));
	}
}

uint32_t extract_bits(FILE* fp) {
	uint32_t word = 0;
	char bit;

	for (int k = 32; k >= 0; k--) {
		bit = getc(fp);
		assert(bit != EOF);
		Bitpack_newu(word, 1, k, bit);
	}
	return word;
}

uint32_t convert_to_bits(Vcs* values)
{
	uint32_t word = 0;
	unsigned a, pb, pr;
	int b, c, d;

	discrete_cos_transform(values, &a, &b, &c, &d);
	quantize_chroma(values, &pb, &pr);

	word = Bitpack_news(word, 4, 0, pr);
	word = Bitpack_news(word, 4, 4, pb);
	word = Bitpack_newu(word, 5, 8, d);
	word = Bitpack_newu(word, 5, 13, c);
	word = Bitpack_newu(word, 5, 18, b);
	word = Bitpack_news(word, 9, 23, a);

	return word;
}


void interpret_bits(Vcs* values, uint32_t word)
{
	unsigned a, pb, pr;
	int b, c, d;

	pr = Bitpack_getu(word, 4, 0);
	pb = Bitpack_getu(word, 4, 4);
	d = Bitpack_gets(word, 5, 8);
	c = Bitpack_gets(word, 5, 13);
	b = Bitpack_gets(word, 5, 18);
	a = Bitpack_getu(word, 9, 23);

	inv_discrete_cos_transform(values, a, b, c, d);
	inv_quantize_chroma(values, pb, pr);

}