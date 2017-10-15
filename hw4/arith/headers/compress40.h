#ifndef COMPRESS40_H
#define COMPRESS40_H

#include <stdio.h>
#include "vcs.h"
#include <pnm.h>

#define A2 A2Methods_UArray2

typedef struct Closure {
	int counter;
	Vcs* values;
	FILE* fp; 
}* Closure;

extern void compress40  (FILE *input);  /* reads PPM, writes compressed image */
extern void decompress40(FILE *input);  /* reads compressed image, writes PPM */

void trim(Pnm_ppm pmap);

void apply_compress(A2Methods_Object *ptr, void *cl);

void apply_decompress(A2Methods_Object *ptr, void *cl);

#endif