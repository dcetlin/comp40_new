/*
 *  COMP 40 HW4
 *  Arith
 *
 *  Jeremy Batchelder UTLN: jbatch01
 *  Daniel Cetlin UTLN: dcetli01
 *
 *  10/20/2017
 *  bitpack.h
 *  Purpose: A suite of functions that get and place bits from/in given
 *           words.
 */

#ifndef BITPACK_INCLUDED
#define BITPACK_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include <except.h>

bool Bitpack_fitsu(uint64_t n, unsigned width);
bool Bitpack_fitss( int64_t n, unsigned width);
uint64_t Bitpack_getu(uint64_t word, unsigned width, unsigned lsb);
int64_t Bitpack_gets(uint64_t word, unsigned width, unsigned lsb);
uint64_t Bitpack_newu(uint64_t word, unsigned width, unsigned lsb,
                      uint64_t value);
uint64_t Bitpack_news(uint64_t word, unsigned width, unsigned lsb,
                      int64_t value);
uint64_t create_mask(unsigned width, unsigned lsb);
extern Except_T Bitpack_Overflow;

#endif
