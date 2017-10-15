#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "vcs.h"

void discrete_cos_transform(Vcs* values, unsigned* a, int* b, int* c, int* d);

void inv_discrete_cos_transform(Vcs* values, unsigned a, int b, int c, int d);

int quantize_coef(float xp);

void quantize_chroma(Vcs* values, unsigned* Pb, unsigned* Pr);

void inv_quantize_chroma(Vcs* values, unsigned pb, unsigned pr);

#endif