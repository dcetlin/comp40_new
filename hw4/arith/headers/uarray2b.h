#ifndef UARRAY2B_INCLUDED
#define UARRAY2B_INCLUDED

#define T UArray2b_T
typedef struct T *T;

/* 
 * 	Creates and returns a UArray2b_T with given height, width, blocksize,
 * 	and size. It is an unchecked runtime error to provide a width, height,
 * 	size, or blocksize to be less than or equal to zero.
 */
extern T    UArray2b_new (int width, int height, int size, int blocksize);

/* 
 * 	Calculates the necessary blocksize such that the total number of bytes 
 *  in a block maximizes but does not exceed 64KB. It then creates and returns
 *  a UArray2b_T with given height, width, size and calculated blocksize.
 *  It is an unchecked runtime error to provide a width, height, or size to
 *  be less than or equal to zero.
 */
extern T    UArray2b_new_64K_block(int width, int height, int size);

/* 
 * 	Takes a pointer to a UArray2b_T and frees the allocated memory. It is a
 *  checked runtime error for the pointer or dereferenced pointer to be NULL.
 */
extern void  UArray2b_free     (T *array2b);

/* 
 * 	Returns the width of the given UArray2b_T. It is a checked runtime error
 *  for the given UArray2b_Tto be NULL.
 */
extern int   UArray2b_width    (T  array2b);

/* 
 * 	Returns the height of the given UArray2b_T. It is a checked runtime error
 *  for the given UArray2b_Tto be NULL.
 */
extern int   UArray2b_height   (T  array2b);

/* 
 * 	Returns the size of the given UArray2b_T. It is a checked runtime error
 *  for the given UArray2b_Tto be NULL.
 */
extern int   UArray2b_size     (T  array2b);

/* 
 * Returns the blocksize of the given UArray2b_T. It is a checked runtime error
 * for the given UArray2b_Tto be NULL.
 */
extern int   UArray2b_blocksize(T  array2b);

/* 
 * 	Returns a void pointer to the element found at the given row and column.
 *  It is an unchecked runtime error to set the value of an element to
 *  something differing in size to the original given size at the creation of
 *  the UArray2b_T. It is a checked runtime error for the given UArray2b_T to
 *  be NULL or for the given row and column to be less than zero or greater
 *  than [height - 1] or [width - 1] respectively.
 */
extern void *UArray2b_at(T array2b, int column, int row);

/* 
 * 	Applies a function to every element in the array looping through each
 *  block in the array before moving on to the next block. It is a checked
 *  runtime error for the given UArray2b_T to be NULL.
 */
extern void  UArray2b_map(T array2b, 
                          void apply(int col, int row, T array2b,
                                     void *elem, void *cl), 
                          void *cl);


#undef T
#endif
