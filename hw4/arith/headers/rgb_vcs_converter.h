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
 *   		 pixel values between Video Component Space and RGB.
 */

#ifndef RGB_VCS_CONVERTER_H
#define RGB_VCS_CONVERTER_H

#include <pnm.h>
#include "a2methods.h"

#define A2 A2Methods_UArray2

/*
 * Converts the given Pnm_ppm’s uarray2 of Pnm_rgb pixel structs to a blocked
 * uarray2b of Vcs component video structs. It is a checked runtime error for
 * pmap to be NULL.
 */
void rgb_to_vcs(Pnm_ppm pmap);

/*
 * Converts the given Pnm_ppm’s blocked uarray2b of Vcs component video
 * structs to a regular uarray2 of Pnm_rgb pixel structs. It is a checked
 * runtime error for pmap to be NULL.
 */
void vcs_to_rgb(Pnm_ppm pmap);

void apply_rgb_to_vcs(int i, int j, A2 array2, A2Methods_Object *ptr,
				      void *cl);

void apply_vcs_to_rgb(int i, int j, A2 array2, A2Methods_Object *ptr,
					  void *cl);

#endif