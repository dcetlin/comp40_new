#ifndef BIT_HANDLER_H
#define BIT_HANDLER_H

void print_bits(uint32_T word);

uint32_t extract_bits(FILE* fp);

uint32_t convert_to_bits(Vcs* values);

void interpret_bits(Vcs* values, uint32_t word);

#endif