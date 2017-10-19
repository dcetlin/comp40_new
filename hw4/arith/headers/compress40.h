/*	
*	COMP 40 HW4
*	Arith
*	
*
*	Jeremy Batchelder UTLN: jbatch01 
*	Daniel Cetlin UTLN: dcetlin01
*
*	10/20/17
*	compress40.h
*	Purpose: This file is the implementation for compress40
*/
#ifndef COMPRESS40_H
#define COMPRESS40_H

/****************************************************************
 *																*	
 *	   Private Functions for compress40 implementation			*
 *																*
 ****************************************************************/

void trim(Pnm_ppm pmap);

void apply_compress(A2Methods_Object *ptr, void *cl);

void apply_decompress(A2Methods_Object *ptr, void *cl);

/*
 * Purpose: Closure struct used in apply_compress and apply_decompress.
 */
typedef struct Closure {
	int counter;
	Vcs* values;
	FILE* fp; 
}* Closure;

extern void compress40  (FILE *input);  
extern void decompress40(FILE *input);  

#endif