#include "headers/uarray2b.h"
#include "headers/uarray2.h"
#include <math.h>


#define T UArray2b_T
#define _64KB 64000

struct T
{
	UArray2_T blocks;
	int width;
	int height;
	int size;
	int blocksize;
};


/*
 * UArray2b_new()
 * Parameters: int width, int height, int size, int blocksize
 * Returns: T
 * Purpose: 
 * 	Creates and returns a UArray2b_T with given height, width, blocksize,
 * 	and size. It is an unchecked runtime error to provide a width, height,
 * 	size, or blocksize to be less than or equal to zero.
 */
T UArray2b_new(int width, int height, int size, int blocksize)
{
	T uarray2b;
	NEW(uarray2b);
	assert(uarray2b);
	uarray2b->width = width;
	uarray2b->height = height;
	uarray2b->size = size;
	uarray2b->blocksize = blocksize;
	uarray2b->blocks = UArray2_new(ceil( (double) width / blocksize),
					   ceil((double) height / blocksize),
					   sizeof(UArray_T));
	for (int i = 0; i < UArray2_width(uarray2b->blocks); i++) {
		for (int j = 0; j < UArray2_height(uarray2b->blocks); j++) {
			*((UArray_T*)UArray2_at(uarray2b->blocks, i, j)) =
				UArray_new(blocksize * blocksize, size);
		}
	}
	return uarray2b;
}

/*
 * UArray2b_free()
 * Parameters: T*
 * Returns: void
 * Purpose: 
 * 	Takes a pointer to a UArray2b_T and frees the allocated memory. It is a
 *  	checked runtime error for the pointer or dereferenced pointer to be
 *	NULL.
 */
void UArray2b_free(T *uarray2b)
{
	assert(uarray2b);
	assert(*uarray2b);

	for (int i = 0; i < UArray2_width((*uarray2b)->blocks); i++) {
		for (int j = 0; j < UArray2_height((*uarray2b)->blocks); j++) {
			UArray_T* uarray_p =
			     (UArray_T*) UArray2_at((*uarray2b)->blocks, i, j);
			UArray_free(uarray_p);
		}
	}

	UArray2_free(&((*uarray2b)->blocks));

	FREE(*uarray2b);
}

/* UArray2b_new_64K_block()
 * Parameters: int int int 
 * Returns: T
 * Purpose: 
 * 	Calculates the necessary blocksize such that the total number of bytes 
 *  in a block maximizes but does not exceed 64KB. It then creates and returns
 *  a UArray2b_T with given height, width, size and calculated blocksize.
 *  It is an unchecked runtime error to provide a width, height, or size to
 *  be less than or equal to zero.
 */
T UArray2b_new_64K_block(int width, int height, int size)
{
	int blocksize = ceil(sqrt((double) _64KB / size));
	return UArray2b_new(width, height, size, blocksize);
}

/*
 * UArray2b_width()
 * Parameters: T
 * Returns: int
 * Purpose: 
 * 	Returns the width of the given UArray2b_T. It is a checked runtime
 *	error for the given UArray2b_Tto be NULL.
 */
int UArray2b_width (T uarray2b)
{
	return uarray2b->width;
}

/*
 * UArray2b_height()
 * Parameters: T
 * Returns: int
 * Purpose: 
 *  Returns the height of the given UArray2b_T. It is a checked runtime error
 *  for the given UArray2b_Tto be NULL.
 */
int UArray2b_height (T uarray2b)
{
	return uarray2b->height;
}

/*
 * UArray2b_size()
 * Parameters: T
 * Returns: int
 * Purpose: 
 *  Returns the size of the given UArray2b_T. It is a checked runtime error
 *  for the given UArray2b_Tto be NULL.
 */
int UArray2b_size (T uarray2b)
{
	return uarray2b->size;
}

/*
 * UArray2b_blocksize()
 * Parameters: T
 * Returns: int
 * Purpose: 
 *  Returns the blocksize of the given UArray2b_T. It is a checked runtime
 *  error for the given UArray2b_Tto be NULL.
 */
int UArray2b_blocksize(T uarray2b)
{
	return uarray2b->blocksize;
}

/*
 * UArray2b_at()
 * Parameters: T, int, int
 * Returns: void*
 * Purpose: 
 *  Returns a void pointer to the element found at the given row and column.
 *  It is an unchecked runtime error to set the value of an element to
 *  something differing in size to the original given size at the creation of
 *  the UArray2b_T. It is a checked runtime error for the given UArray2b_T to
 *  be NULL or for the given row and column to be less than zero or greater
 *  than [height - 1] or [width - 1] respectively.
 */
void* UArray2b_at(T uarray2b, int col, int row)
{
	assert(uarray2b);
	assert(row >= 0 && row < uarray2b->height);
	assert(col >= 0 && col < uarray2b->width);

	int blocksize = uarray2b->blocksize;

	return UArray_at(*((UArray_T*) UArray2_at(uarray2b->blocks,
		col / blocksize, row / blocksize)),
		blocksize * (col % blocksize) + row % blocksize);
}

/*
 * UArray2b_map()
 * Parameters: T, void apply(int, int, T, void*, void*), void*
 * Returns: void
 * Purpose: 
 *  Applies a function to every element in the array looping through each
 *  block in the array before moving on to the next block. It is a checked
 *  runtime error for the given UArray2b_T to be NULL.
 */
void UArray2b_map(T uarray2b, void apply(int col, int row, T uarray2b,
	void *elem, void *cl), void *cl)
{
	assert(uarray2b);
	int blocksize = uarray2b->blocksize;

	for(int i = 0; i < UArray2_width(uarray2b->blocks); i++) {
		for(int j = 0; j < UArray2_height(uarray2b->blocks); j++) {
			for (int k = 0; k < pow(uarray2b->blocksize, 2); k++) {
				int col = k % blocksize + i * blocksize;
				int row = k / blocksize + j * blocksize;
				if (row >= 0 && row < uarray2b->height && 
					col >= 0 && col < uarray2b->width) {
					void* cell = UArray2b_at(uarray2b,
								 col, row);
					apply(col, row, uarray2b, cell, cl);
				}
			}
		}
	}
}