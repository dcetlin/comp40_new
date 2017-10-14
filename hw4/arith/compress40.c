#include "compress40.h"
#include "headers/vcs.h"
#include "headers/rgb_vcs_converter.h"
#include <pnm.h>
#include "headers/a2methods.h"
#include "headers/a2plain.h"
#include "headers/a2blocked.h"
#include <stdbool.h>

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
	vcs_to_rgb(pmap);

	Pnm_ppmwrite(stdout, pmap);
/*
	Closure cls;
	cls->counter = 0;
	pmap->methods->map_default(pmap, apply_compress, (void*) cls);
*/
	Pnm_ppmfree(&pmap);
}

extern void decompress40(FILE *input) {
	(void) input;
}



/*
void apply_compress(int i, int j, A2 array2, A2Methods_Object *ptr, void *cl) {

}

void apply_decompress(int i, int j, A2 array2, A2Methods_Object *ptr, void *cl)
*/