/*	
*	COMP 40 HW4
*	Arith
*	
*
*	Jeremy Batchelder UTLN: jbatch01 
*	Daniel Cetlin UTLN: dcetlin01
*
*	10/20/17
*	bit_handler.c
*	Purpose: This file is the implementation of the bit_handler interface.
*/
#include "headers/bit_handler.h"
#include "headers/algorithms.h"
#include "headers/bitpack.h"
#include <assert.h>
#include <stdio.h>

/*
 *	Purpose: Prints the given word in binary in Big Endian format.
 *	Parameters: A single 32 bit word
 *	Returns: None
 *	CRE: None
 */
void print_bits(uint32_t word)
{
	for (int bit_loc = 24; bit_loc >= 0; bit_loc -= 8) {
		putchar((char) Bitpack_getu(word, 8, bit_loc));
	}
}

/*
 *	Purpose: Reads in 32 bits from the given Big Endian formatted binary
 *		file and returns a word filled with the 32 bits.
 *	Parameters: File pointer
 *	Returns: A single 32 bit word which will contain the 
 *   	bits read in from the file
 *	CRE:  If at any point, the end of file character is read in, it would
 *   	imply that the given file is either too short, or the last word is
 *  	incomplete. In either case, the program will fail with a Checked 
 *   	Runtime Error.
 *
 */
uint32_t extract_bits(FILE* fp) {
	uint32_t word = 0;

	signed byte;
	for (int bit_loc = 24; bit_loc >= 0; bit_loc -= 8) {
		byte = getc(fp);
		assert(byte != EOF);
		word = Bitpack_newu(word, 8, bit_loc, byte);
	}
	return word;
}

/*
 *	Purpose: This function converts, quantizes, and compresses the input 
 *		values and returns them in bits.
 *	Parameters: A pointer to an array of Vcs structs 
 *	Returns: The arguments in a single 32 bit word. 
 *	CRE: None
 */
uint32_t convert_to_bits(Vcs* values)
{
	uint32_t word = 0;
	unsigned a, pb, pr;
	int b, c, d;

	discrete_cos_transform(values, &a, &b, &c, &d);
	quantize_chroma(values, &pb, &pr);

	word = Bitpack_newu(word, 4, 0, pr);
	word = Bitpack_newu(word, 4, 4, pb);
	word = Bitpack_news(word, 5, 8, d);
	word = Bitpack_news(word, 5, 13, c);
	word = Bitpack_news(word, 5, 18, b);
	word = Bitpack_newu(word, 9, 23, a);

	return word;
}

/*
 *	Purpose: This function decompresses, quantizes, and converts values
 *		and places them in the values argument
 *	Parameters: A pointer to an array of Vcs structs and a single 32 bit word 
 *	Returns: The arguments in a single 32 bit word. 
 *	CRE: None
 */
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