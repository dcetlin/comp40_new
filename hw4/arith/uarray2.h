/*
*
*  COMP 40 HW2
*  Interfaces, Implementations, and Images
*
*  Daniel Grichevsky UTLN: dgrich01
*  Daniel Cetlin UTLN: dcetli01
*
*  09/17/2017
*  uarray2.h
*  Purpose: Interface for the unboxed 2-d array
*/

#include <uarray.h>
#include <mem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef UARRAY2_INCLUDED
#define UARRAY2_INCLUDED

#define T UArray2_T
typedef struct T *T;

extern void * UArray2_at(T uarray2, int col, int row);
extern T UArray2_new(int width, int height, int size);
extern void UArray2_free(T *uarray2);
extern int UArray2_width(T uarray2);
extern int UArray2_height(T uarray2);
extern int UArray2_size(T uarray2);
extern void UArray2_map_col_major(T uarray2, void apply(int col, int row,
         T uarray2, void * elem, void *cl), void *cl);
extern void UArray2_map_row_major(T uarray2, void apply(int col, int row,
         T uarray2, void * elem, void *cl), void *cl);

#undef T
#endif