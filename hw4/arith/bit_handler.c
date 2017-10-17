#include "headers/bit_handler.h"
#include "headers/algorithms.h"
#include "headers/bitpack.h"
#include <assert.h>
#include <stdio.h>

void print_bits(uint32_t word)
{
	for (int k = 31; k >= 0; k--) {
		printf("%u", (unsigned) Bitpack_getu(word, 1, k));
	}
}

uint32_t extract_bits(FILE* fp) {
	uint32_t word = 0;
	char bit;

	for (int k = 31; k >= 0; k--) {
		bit = getc(fp);
		//printf("(%c", bit);
		assert (bit == '0' || bit == '1');
		
		if (bit == '0') {
			bit = 0;
		} else {
			bit = 1;
		}

		assert(bit != EOF);
		word = Bitpack_newu(word, 1, k, bit);
	}

	//printf("word{%08x}\n", word);
	return word;
}

uint32_t convert_to_bits(Vcs* values)
{
	uint32_t word = 0;
	unsigned a, pb, pr;
	int b, c, d;

	discrete_cos_transform(values, &a, &b, &c, &d);
	quantize_chroma(values, &pb, &pr);

	//printf("pr: %i | pb = %i | d = %u | c = %u | b = %u | a = %i\n", pr, pb, d, c, b, a);

	word = Bitpack_newu(word, 4, 0, pr);
	word = Bitpack_newu(word, 4, 4, pb);
	word = Bitpack_news(word, 5, 8, d);
	word = Bitpack_news(word, 5, 13, c);
	word = Bitpack_news(word, 5, 18, b);
	word = Bitpack_newu(word, 9, 23, a);

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