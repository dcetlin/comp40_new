/*
 *  COMP 40 HW4
 *  Arith
 *
 *  Jeremy Batchelder UTLN: jbatch01
 *  Daniel Cetlin UTLN: dcetli01
 *
 *  10/20/2017
 *  rgb_vcs_converter.h
 *  Purpose: Two public functions that take in a pmap and convert all of the
 *           pixel values between Video Component Space and RGB.
 */

#include "rgb_vcs_converter.h"
#include "vcs.h"
#include <pnm.h>
#include <stdio.h>
#include "a2plain.h"
#include "a2blocked.h"
#include <math.h>
#include <assert.h>

typedef A2Methods_mapfun* MapFun;

/*
 * rgb_to_vcs
 * Parameters: Pnm_ppm
 * Return: N/A
 * Purpose: Converts the given Pnm_ppm’s uarray2 of Pnm_rgb pixel structs to a
 * blocked uarray2b of Vcs component video structs. It is a checked runtime
 * error for pmap to be NULL.
 */
void rgb_to_vcs(Pnm_ppm pmap)
{
        assert(pmap != NULL);

        A2 new_pixmap = uarray2_methods_blocked->
                new_with_blocksize(pmap->width, pmap->height, sizeof(Vcs), 2);

        uarray2_methods_blocked->map_default(new_pixmap, apply_rgb_to_vcs,
                                             pmap);

        pmap->methods->free(&(pmap->pixels));
        pmap->pixels = new_pixmap;
        pmap->methods = uarray2_methods_blocked;
}

/*
 * vcs_to_rgb
 * Parameters: Pmap
 * Return: N/A
 * Purpose: Converts the given Pnm_ppm’s blocked uarray2b of Vcs component
 *          video structs to a regular uarray2 of Pnm_rgb pixel structs. It
 *          is a checked runtime error for pmap to be NULL.
 */
void vcs_to_rgb(Pnm_ppm pmap)
{
        assert(pmap != NULL);

        A2 new_pixmap = uarray2_methods_plain->
                new(pmap->width, pmap->height, sizeof(struct Pnm_rgb));

        uarray2_methods_plain->map_default(new_pixmap, apply_vcs_to_rgb, pmap);

        pmap->methods->free(&(pmap->pixels));
        pmap->pixels = new_pixmap;
        pmap->methods = uarray2_methods_plain;
}

/*
 * apply_rgb_to_vcs
 * Parameters: int, int, A2, A2Methods_Object*, void*
 * Return: N/A
 * Purpose: The apply function converts the Pnm_ppm pixel value ptr into a Vcs
 *          struct and places the Vcs struct at position i and j in the
 *          uarray2b contained within in the closure.
 */
void apply_rgb_to_vcs(int i, int j, A2 array2, A2Methods_Object *ptr, void *cl)
{
        (void) array2;
        A2 old_pixmap = ((Pnm_ppm) cl)->pixels;
        int denom = ((Pnm_ppm) cl)->denominator;
        
        Pnm_rgb rgb = (Pnm_rgb)((Pnm_ppm) cl)->methods->at(old_pixmap, i, j);

        Vcs pixel_info = {
                .y = (0.299 * rgb->red + 0.587 * rgb->green
                                + 0.114 * rgb->blue) / denom,
                .pb = (-0.168736 * rgb->red - 0.331264 * rgb->green
                                + 0.5 * rgb->blue) / denom,
                .pr = (0.5 * rgb->red - 0.418688 * rgb->green
                                - 0.081312 * rgb->blue) / denom};
        
        *(Vcs*)ptr = pixel_info;
}

/*
 * apply_vcs_to_rgb
 * Parameters: int, int, A2, A2Methods_Object*, void*
 * Return: N/A
 * Purpose: The apply function converts the Vcs struct value in ptr into a
 *          Pnm_rgb pixel value and places the Pnm_rgb  struct at position i
 *          and j in the uarray2 contained within in the closure. The range of
 *          each RGB values is 0 to denominator (which is expected to be 255).
 */
void apply_vcs_to_rgb(int i, int j, A2 array2, A2Methods_Object *ptr, void *cl)
{
        (void) array2;
        A2 old_pixmap = ((Pnm_ppm) cl)->pixels;
        int denom = ((Pnm_ppm) cl)->denominator;
        
        Vcs vcs = *((Vcs*)uarray2_methods_blocked->at(old_pixmap, i, j));

        int red   = round((1.0 * vcs.y + 0 * vcs.pb + 1.402 * vcs.pr)
                           * denom);
        int green = round((1.0 * vcs.y - 0.344136 * vcs.pb - 0.714136 * vcs.pr)
                           * denom);
        int blue  = round((1.0 * vcs.y + 1.772 * vcs.pb + 0.0 * vcs.pr)
                           * denom);
        struct Pnm_rgb pixel_info = {
                .red   = (red < 0)  ? 0: (red > 255)  ? 255: (unsigned) red,
                .green = (green < 0)? 0: (green > 255)? 255: (unsigned) green,
                .blue  = (blue < 0) ? 0: (blue > 255) ? 255: (unsigned) blue};

        *((Pnm_rgb)ptr) = pixel_info;
}