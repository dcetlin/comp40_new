#include <compress40.h>
#include "headers/rgb_vcs_converter.h"
#include <pnm.h>
#include "headers/a2methods.h"
#include "headers/a2plain.h"
#include "headers/a2blocked.h"
#include "headers/bit_handler.h"
#include <stdbool.h>
#include <assert.h>
#include <mem.h>

#define A2 A2Methods_UArray2

void trim(Pnm_ppm pmap);

void apply_compress(A2Methods_Object *ptr, void *cl);

void apply_decompress(A2Methods_Object *ptr, void *cl);

typedef struct Closure {
	int counter;
	Vcs* values;
	FILE* fp; 
}* Closure;

void trim(Pnm_ppm pmap) 
{
	if (pmap->height % 2 != 0 ) {
		pmap->height--;
	}
	if (pmap->width % 2 != 0 ) {
		pmap->width--;
	}
}

extern void compress40  (FILE *input)
{	
	Pnm_ppm pmap = Pnm_ppmread(input, uarray2_methods_plain);
	trim(pmap);

	rgb_to_vcs(pmap);

	Pnm_ppmwrite(stdout, pmap);

	Closure cls = ALLOC(sizeof(*cls));
	assert(cls);
	cls->counter = 0;
	cls->values = CALLOC(4, sizeof(Vcs));
	cls->fp = NULL;
	
	printf("COMP40 Compressed image format 2\n%u %u\n",
			pmap->width, pmap->height);

	pmap->methods->small_map_default(pmap, apply_compress, (void*) cls);

	FREE((cls->values));
	FREE(cls);

	Pnm_ppmfree(&pmap);
}

extern void decompress40(FILE *input) {
	unsigned height, width;
	int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", &width, &height);
	assert(read == 2);
	int c = getc(input);
	assert(c == '\n');

	A2 array = uarray2_methods_blocked->new_with_blocksize(width, height, sizeof(Vcs), 2);

	struct Pnm_ppm pmap = {.width = width, .height = height, .denominator = 255,
							.methods = uarray2_methods_blocked,
							.pixels = array
	};

	Closure cls = ALLOC(sizeof(*cls));
	assert(c == '\n');
	cls->counter = 0;
	cls->values = CALLOC(4, sizeof(Vcs));
	cls->fp = input;

	pmap.methods->small_map_default(pmap.pixels, apply_decompress, (void*) cls);

	vcs_to_rgb(&pmap);
	Pnm_ppmwrite(stdout, &pmap);

	FREE((cls->values));
	FREE(cls);
	fclose(input);

	pmap.methods->free(&array);
}




void apply_compress(A2Methods_Object *ptr, void *cl) {
	Closure cls = (Closure) cl;
	Vcs pixel = *((Vcs*) ptr);

	if (cls->counter > 3) {
		cls->counter = 0;
		print_bits(convert_to_bits(cls->values));
	}

	cls->values[cls->counter] = pixel;
	cls->counter++;
}

void apply_decompress(A2Methods_Object *ptr, void *cl)
{
	Closure cls = (Closure) cl;
	if (cls->counter == 0) {
		interpret_bits(cls->values, extract_bits(cls->fp));
	}
	
	*((Vcs*) ptr) = cls->values[cls->counter];
	
	cls->counter = (cls->counter + 1) % 4;
}