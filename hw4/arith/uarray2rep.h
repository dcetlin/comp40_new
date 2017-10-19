/*
*
*  COMP 40 HW2
*  Interfaces, Implementations, and Images
*
*  Daniel Grichevsky UTLN: dgrich01
*  Daniel Cetlin UTLN: dcetli01
*
*  09/17/2017
*  uarray2rep.h
*  Purpose: Private header file for initializing the implementation
*  of the unboxed array
*/

#ifndef UARRAY2REP_INCLUDED
#define UARRAY2REP_INCLUDED
#include <uarray.h>

#define T UArray2_T
struct T {
        int height;
        int width;
        UArray_T array_1d;
};

extern void UArray2Rep_init(T uarray2, int width, int height, int size);

#undef T
#endif