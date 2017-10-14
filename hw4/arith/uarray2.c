/*
*  COMP 40 HW2
*  Interfaces, Implementations, and Images
*
*  Daniel Grichevsky UTLN: dgrich01
*  Daniel Cetlin UTLN: dcetli01
*
*  09/17/2017
*  uarray2.c
*  Purpose: UArray2_T represents a 2-d array containing various functions
*             that manipulate the contents of the array.
*/

#include "headers/uarray2.h"
#include "headers/uarray2rep.h"

#define T UArray2_T

/*
        Declaration found in "uarray2rep.h" - private function
        void UArray2Rep_init(T uarray2, int width, int height, int size)
        Purpose: Helper function to UArray2_new that initializes private struct
        Arguments: T, int, int, int
        Return: N/A
*/
void UArray2Rep_init(T uarray2, int width, int height, int size)
{
        uarray2->height = height;
        uarray2->width = width;
        uarray2->array_1d = UArray_new(height * width,  size);
}

/*
        T UArray2_new(int width, int height)
        Purpose: Creates and returns an allocated 2-d array where each element
        has the given size
        Arguments: int, int int
        Return: T
*/
T UArray2_new(int width, int height, int size)
{
        T uarray2;
        NEW(uarray2);
        assert(uarray2);
        UArray2Rep_init(uarray2, width, height, size);
        return uarray2;
}

/*
        void UArray2_free(T *UArray2)
        Purpose: Free's all of the allocated memory of the 2-d array
        Arguments: T *
        Return: N/A
*/
void UArray2_free(T *uarray2)
{
        assert(uarray2 && *uarray2);
        assert((*uarray2)->array_1d);
        UArray_free(&(*uarray2)->array_1d);
        FREE(*uarray2);
}

/*
        int UArray2_width(T UArray2)
        Purpose: Returns the width of the 2-d array of bits
        Arguments: T
        Return: int
*/
int UArray2_width(T uarray2)
{
        assert(uarray2);
        return uarray2->width;
}

/*
        int UArray2_height(T UArray2)
        Purpose: Returns the height of the 2-d array of bits
        Arguments: T
        Return: int
*/
int UArray2_height(T uarray2)
{
        assert(uarray2);
        return uarray2->height;
}

/*
        int UArray2_size(T UArray2)
        Purpose: Returns the size of an element
        Arguments: T
        Return: int
*/
int UArray2_size(T uarray2)
{
        assert(uarray2);
        return UArray_size(uarray2->array_1d);
}

/*
        void* UArray2_at(T UArray2, int col, int row)
        Purpose: Returns a pointer to the contents at the given row and col
        Arguments: T, int, int
        Return: void*
*/
void * UArray2_at(T uarray2, int col, int row)
{
        assert(uarray2);
        assert(row >= 0 && row < uarray2->height);
        assert(col >= 0 && col < uarray2->width);

        return UArray_at(uarray2->array_1d, row * uarray2->width + col);
}

/*
        int UArray2_map_row_major(T UArray2, void apply(int col, int row,
        T UArray2, void* elem, void *cl), void *cl)
        Purpose: Applies a function to every element in the array
        looping through each column in the array before the rows
        Arguments: T, void apply(), void *
        Return: N/A
*/
void UArray2_map_row_major(T uarray2, void apply(int col, int row, T uarray2,
         void * elem, void *cl), void *cl)
{
        assert(uarray2);
        for (int i = 0;  i < UArray_length(uarray2->array_1d); i++) {
                apply( (i % uarray2->width), (i / uarray2->width), uarray2,
                 UArray_at(uarray2->array_1d, i), cl);
        }
}

/*
        int UArray2_map_col_major(T UArray2, void apply(int col, int row,
         T UArray2, void* elem, void *cl), void *cl)
        Purpose: Applies a function to every element in the array
        looping through each row in the array before the columns
        Arguments: T, void apply(), void *
        Return: N/A
*/
void UArray2_map_col_major(T uarray2, void apply(int col, int row, T uarray2,
         void * elem, void *cl), void *cl)
{
        assert(uarray2);
        for (int i = 0;  i < UArray_length(uarray2->array_1d); i++) {
                apply( (i / uarray2->height), (i % uarray2->height), uarray2,
                UArray_at(uarray2->array_1d,  uarray2->width *
                        (i %  uarray2->height) + (i /  uarray2->height)), cl);
        }
}
