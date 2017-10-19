/*
 *  COMP 40 HW4
 *  Arith
 *
 *  Jeremy Batchelder UTLN: jbatch01
 *  Daniel Cetlin UTLN: dcetli01
 *
 *  10/20/2017
 *  bitpack.c
 *  Purpose: A suite of functions that get and place bits from/in given
 *   	     words.
 */

#include "bitpack.h"
#include <assert.h>
#include <stdio.h>
Except_T Bitpack_Overflow = { "Overflow packing bits" };

/* 
 * Bitpack_fitsu
 * Parameters: uint64_t, unsigned
 * Returns: bool
 * Purpose: Returns true if the the unsigned argument n can be represented
 * 	    in width bits.
 * CRE: It is a checked runtime error for width to be greator than 64.
 */
bool Bitpack_fitsu(uint64_t n, unsigned width)
{
	if (width > 64) {
		RAISE(Bitpack_Overflow);
	}

	uint64_t max = ~0;
	max >>= 64 - width;

	return (n <= max && width);
}

/* 
 * Bitpack_fitss
 * Parameters: uint64_t, unsigned
 * Returns: bool
 * Purpose: Returns true if the the signed argument n can be represented
 * 	    in width bits.
 * CRE: It is a checked runtime error for width to be greator than 64.
 */
bool Bitpack_fitss(int64_t n, unsigned width)
{
	if (width > 64) {
		RAISE(Bitpack_Overflow);
	}

	int64_t min = ((int64_t) 1 << (63)) >> (64 - width);
	int64_t max = ((int64_t) 1 << (width - 1)) - 1;

	return (n >= min && n <= max && width);
}

/* 
 * create_mask
 * Parameters: unsigned, unsigned
 * Returns: uint646_t
 * Purpose: Helper function in get and new functions. Creates a uint64_t
 *	    where each bit starting at index lsb up to lsb + width is on (1),
 *	    and the rest of the bits are off (0).
 */
uint64_t create_mask(unsigned width, unsigned lsb) 
{
	uint64_t mask = ~0;
	mask = (((mask << (64 - width - lsb)) >> (64 - width)) << lsb);
	
	return mask;
}

/* 
 * Bitpack_getu
 * Parameters: uint64_t, unsigned, unsigned
 * Returns: uint64_t
 * Purpose: Extracts a field from a word given the width of the field and the
 *  	    location of the field’s least significant bit. The field is treated
 *          as a non-negative value.
 * CRE: It is a checked runtime error for width to be greator than 64 and
 * 	for the index of width + lsb to exceed 64.
 */
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb) 
{
	if (width > 64 && width + lsb > 64) {
		RAISE(Bitpack_Overflow);
	}

	uint64_t mask = create_mask(width, lsb);

	return (word & mask) >> (lsb);
}

/* 
 * Bitpack_fitsu
 * Parameters: uint64_t, unsigned
 * Returns: int64_t
 * Purpose: Extracts a field from a word given the width of the field and the
 *  	    location of the field’s least significant bit. The field is treated
 *          as a signed value.
 * CRE: It is a checked runtime error for width to be greator than 64 and
 * 	for the index of width + lsb to exceed 64.
 * Implementation notes: If the value is negative, this function will 
 * 			 return the value at index lsb of size width 
 			 represented in 64 bits. (Leading 1's or leading 0's
 			 will be propogated as necessary).
 */
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb)
{
	if (width > 64 && width + lsb > 64) {
		RAISE(Bitpack_Overflow);
	}

	uint64_t mask = create_mask(width, lsb);

	return ((int64_t)(word & mask) << (64 - width - lsb)) >> (64 - width);
}

/* 
 * Bitpack_newu
 * Parameters: uint64_t, unsigned, unsigned, uint64_t
 * Returns: uint64_t
 * Purpose: Return an updated version of the given uint64_t word such that 
 *	    the bits at index lsb and of length width are replaced with those
 *	    of value. Value is treated as a non-negative number.
 * CRE: It is a checked runtime error for width to be greator than 64 and
 * 	for the index of width + lsb to exceed 64. It is also a checked runtime
 * 	error for value to be unable to be represented in width bits.
 */
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb,
		      uint64_t value)
{
	if ((width > 64 && width + lsb > 64) || !Bitpack_fitsu(value, width)) {
		RAISE(Bitpack_Overflow);
	}

	uint64_t mask = ~create_mask(width, lsb);

	return (word & mask) | (value << lsb);
}

/* 
 * Bitpack_news
 * Parameters: uint64_t, unsigned, unsigned, int64_t
 * Returns: uint64_t
 * Purpose: Return an updated version of the given uint64_t word such that 
 *	    the bits at index lsb and of length width are replaced with those
 *	    of value. Value is treated as a signed number.
 * CRE: It is a checked runtime error for width to be greator than 64 and
 * 	for the index of width + lsb to exceed 64. It is also a checked runtime
 * 	error for value to be unable to be represented in width bits.
 * Implementation notes: Because the value can have many leading zeros or 1's,
 *			 our implementation will propagate as many leading 
 *			 values as necessary to satisfy the width. That is why
 *			 we create a mask the sice of width in order to 
 *			 trunkate any unneccessary 1's of a negative number.
 */
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,
		      int64_t value)
{
	if ((width > 64 && width + lsb > 64) || !Bitpack_fitss(value, width)) {
		RAISE(Bitpack_Overflow);
	}

	uint64_t mask = ~create_mask(width, lsb);

	return (word & mask) | ((value & (create_mask(width, 0))) << lsb);
}