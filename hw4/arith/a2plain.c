#include <stdlib.h>

#include <a2plain.h>
#include "uarray2.h"

typedef A2Methods_UArray2 A2;	// private abbreviation

// define a private version of each function in A2Methods_T that we implement
//comment

static A2 new(int width, int height, int size)
{
	return UArray2_new(width, height, size); 
}

static A2 new_with_blocksize(int width, int height, int size,
					    int blocksize)
{
	(void)blocksize;
	return UArray2_new(width, height, size);
}

static void a2free(A2 * array2p)
{
	UArray2_free((UArray2_T *) array2p);
}

static int width(A2 array2)
{
	return UArray2_width(array2);
}
static int height(A2 array2)
{
	return UArray2_height(array2);
}
static int size(A2 array2)
{
	return UArray2_size(array2);
}

static int blocksize(A2 array2)
{
	(void) array2;
	return 1;
}

static A2Methods_Object *at(A2 array2, int i, int j)
{
	return UArray2_at(array2, i, j);
}

typedef void UArray2_apply(int row, int col, UArray2_T a2, void *elem, void *cl);
extern void UArray2_map_row_major(UArray2_T a2, UArray2_apply apply, void *cl);

struct a2fun_closure {
	A2Methods_applyfun *apply; /* apply function as known to A2Methods */
	void *cl; /* closure to go with apply function */
	A2 array2; /* array being mapped over */
};

static void apply_a2methods_using_array2_prototype(int row, int col, UArray2_T a2,
	void *elem, void *cl)
{
		(void) a2;
		struct a2fun_closure *f = cl; /* function/closure originally passed */
		f->apply(row, col, f->array2, elem, f->cl);
}

static void map_row_major(A2 array2, A2Methods_applyfun apply,
	void *cl)
{
		struct a2fun_closure mycl = { apply, cl, array2 };
		UArray2_map_row_major(array2, apply_a2methods_using_array2_prototype,
			&mycl);
}

extern void UArray2_map_col_major(UArray2_T a2, UArray2_apply apply, void *cl);

static void map_col_major(A2 array2, A2Methods_applyfun apply,
	void *cl)
{
		struct a2fun_closure mycl = { apply, cl, array2 };
		UArray2_map_col_major(array2, apply_a2methods_using_array2_prototype,
			&mycl);
}

struct a2fun_small_closure {
	A2Methods_smallapplyfun *apply;
	void *cl;
};

static void apply_small(int i, int j, UArray2_T array2, void *elem, void *vcl)
{
	struct a2fun_small_closure *cl = vcl;
	(void)i;
	(void)j;
	(void)array2;
	cl->apply(elem, cl->cl);
}

static void small_map_row_major(A2 a2, A2Methods_smallapplyfun apply,
				  void *cl)
{
	struct a2fun_small_closure mycl = { apply, cl };
	UArray2_map_row_major(a2, apply_small, &mycl);
}

static void small_map_col_major(A2 a2, A2Methods_smallapplyfun apply,
				  void *cl)
{
	struct a2fun_small_closure mycl = { apply, cl };
	UArray2_map_col_major(a2, apply_small, &mycl);
}

static void small_map_default(A2 a2, A2Methods_smallapplyfun apply,
				  void *cl)
{
	struct a2fun_small_closure mycl = { apply, cl };
	UArray2_map_row_major(a2, apply_small, &mycl);
}

static void map_default(A2 array2, A2Methods_applyfun apply,
	void *cl)
{
		struct a2fun_closure mycl = { apply, cl, array2 };
		UArray2_map_row_major(array2, apply_a2methods_using_array2_prototype,
			&mycl);
}

static struct A2Methods_T uarray2_methods_plain_struct = {
	new,
	new_with_blocksize,
	a2free,
	width,
	height,
	size,
	blocksize,
	at,
	map_row_major,		// map_row_major
	map_col_major,		// map_col_major
	NULL,
	map_default,		// map_default
	small_map_row_major,	// small_map_row_major
	small_map_col_major,	// small_map_col_major
	NULL,
	small_map_default,	// small_map_default
};

// finally the payoff: here is the exported pointer to the struct

A2Methods_T uarray2_methods_plain = &uarray2_methods_plain_struct;
