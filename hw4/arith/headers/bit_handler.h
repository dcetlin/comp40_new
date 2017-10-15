#ifndef BIT_HANDLER_H
#define BIT_HANDLER_H

#include "vcs.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


void print_bits(uint32_t word);

uint32_t extract_bits(FILE* fp);

uint32_t convert_to_bits(Vcs* values);

void interpret_bits(Vcs* values, uint32_t word);

#endif