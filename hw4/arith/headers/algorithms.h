/*
 *  COMP 40 HW4
 *  Arith
 *
 *  Jeremy Batchelder UTLN: jbatch01
 *  Daniel Cetlin UTLN: dcetli01
 *
 *  10/20/2017
 *  algorithms.h
 *  Purpose: Private functions used to compute quantinizations on each part of
 *           a Video Component Space pixel (Y, Pr, Pb).
 */

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "vcs.h"

void discrete_cos_transform(Vcs* values, unsigned* a, int* b, int* c, int* d);

void inv_discrete_cos_transform(Vcs* values, unsigned a, int b, int c, int d);

int quantize_coef(float xp);

void quantize_chroma(Vcs* values, unsigned* Pb, unsigned* Pr);

void inv_quantize_chroma(Vcs* values, unsigned pb, unsigned pr);

#endif