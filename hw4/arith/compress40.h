#ifndef COMPRESS40_H
#define COMPRESS40_H

#include <stdio.h>

typedef struct Closure {
	int counter;
	Vcs* values;
	FILE* fp; 
}* Closure;

extern void compress40  (FILE *input);  /* reads PPM, writes compressed image */
extern void decompress40(FILE *input);  /* reads compressed image, writes PPM */

void trim(Pnm_ppm pmap);

void apply_compress(int i, int j, A2 array2, A2Methods_Object *ptr, void *cl)

void apply_decompress(int i, int j, A2 array2, A2Methods_Object *ptr, void *cl)

#endif