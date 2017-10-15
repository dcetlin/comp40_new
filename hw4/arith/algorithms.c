#include "headers/algorithms.h"
#include <arith40.h>
#include <math.h>

void discrete_cos_transform(Vcs* values, unsigned* a, int* b, int* c, int* d)
{
	float a_f = (values[3].y + values[2].y + values[1].y + values[0].y) / 4.0;
	float b_f = (values[3].y + values[2].y - values[1].y - values[0].y) / 4.0;
	float c_f = (values[3].y - values[2].y + values[1].y - values[0].y) / 4.0;
	float d_f = (values[3].y - values[2].y - values[1].y + values[0].y) / 4.0;

	*a = (unsigned) round(a_f * 511);
	*b = quantize_coef(b_f);
	*c = quantize_coef(c_f);
	*d = quantize_coef(d_f);
}

void inv_discrete_cos_transform(Vcs* values, unsigned a, int b, int c, int d)
{
	float a_f = a / 511.0;
	float b_f = b / 50.0;
	float c_f = c / 50.0;
	float d_f = d / 50.0;

	values[0].y = a_f - b_f - c_f + d_f;
	values[1].y = a_f - b_f + c_f - d_f;
	values[2].y = a_f + b_f - c_f - d_f;
	values[3].y = a_f + b_f + c_f + d_f;
}

int quantize_coef(float x)
{
	if (x > 0.3)
		x = 0.3;
	if (x < -0.3)
		x = -0.3;
	return round(x * 50);
}

void quantize_chroma(Vcs* values, unsigned* i_pb, unsigned* i_pr)
{
	float pb_f = ((values[0].pb + values[1].pb + values[2].pb +
		values[3].pb) / 4.0);
	float pr_f = ((values[0].pr + values[1].pr + values[2].pr +
		values[3].pr) / 4.0);

	*i_pb = Arith40_index_of_chroma(pb_f);
	*i_pr = Arith40_index_of_chroma(pr_f);
}

void inv_quantize_chroma(Vcs* values, unsigned pb, unsigned pr)
{
	float pb_f = Arith40_chroma_of_index(pb);
	float pr_f = Arith40_chroma_of_index(pr);

	for (unsigned k = 0; k < 4; k++) {
		values[k].pb = pb_f;
		values[k].pr = pr_f;
	}
}