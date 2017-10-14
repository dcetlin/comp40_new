#include "headers/rgb_vcs_converter.h"
#include "headers/vcs.h"
#include <pnm.h>
#include <stdio.h>
#include "headers/a2plain.h"
#include "headers/a2blocked.h"
#include <math.h>

typedef A2Methods_mapfun* MapFun;

void rgb_to_vcs(Pnm_ppm pmap)
{
	A2 new_pixmap = uarray2_methods_blocked->
		new_with_blocksize(pmap->width, pmap->height, sizeof(Vcs), 2);

	uarray2_methods_blocked->map_default(new_pixmap, apply_rgb_to_vcs, pmap);

	pmap->methods->free(&(pmap->pixels));
	pmap->pixels = new_pixmap;
	pmap->methods = uarray2_methods_blocked;
}

void vcs_to_rgb(Pnm_ppm pmap)
{
	A2 new_pixmap = uarray2_methods_plain->
		new(pmap->width, pmap->height, sizeof(struct Pnm_rgb));

	uarray2_methods_plain->map_default(new_pixmap, apply_vcs_to_rgb, pmap);

	pmap->methods->free(&(pmap->pixels));
	pmap->pixels = new_pixmap;
	pmap->methods = uarray2_methods_plain;
}

void apply_rgb_to_vcs(int i, int j, A2 array2, A2Methods_Object *ptr, void *cl)
{
	(void) array2;
	A2 old_pixmap = ((Pnm_ppm) cl)->pixels;
	int denom = ((Pnm_ppm) cl)->denominator;
	
	Pnm_rgb rgb = (Pnm_rgb)((Pnm_ppm) cl)->methods->at(old_pixmap, i, j);

	Vcs pixel_info = { .y = (0.299 * rgb->red + 0.587 * rgb->green + 0.114 * rgb->blue) / denom,
		.pb = (-0.168736 * rgb->red - 0.331264 * rgb->green + 0.5 * rgb->blue) / denom,
		.pr = (0.5 * rgb->red - 0.418688 * rgb->green - 0.081312 * rgb->blue) / denom};
	*(Vcs*)ptr = pixel_info;
}

void apply_vcs_to_rgb(int i, int j, A2 array2, A2Methods_Object *ptr, void *cl)
{
	(void) array2;
	A2 old_pixmap = ((Pnm_ppm) cl)->pixels;
	int denom = ((Pnm_ppm) cl)->denominator;
	
	Vcs vcs = *((Vcs*)uarray2_methods_blocked->at(old_pixmap, i, j));

	struct Pnm_rgb pixel_info = {.red = round((1.0 * vcs.y + 0 * vcs.pb + 1.402 * vcs.pr) * denom),
		.green = round((1.0 * vcs.y - 0.344136 * vcs.pb - 0.714136 * vcs.pr) * denom),
		.blue = round((1.0 * vcs.y + 1.772 * vcs.pb + 0.0 * vcs.pr) * denom) };	

	*((Pnm_rgb)ptr) = pixel_info;
}