/*      
*       COMP 40 HW4
*       Arith
*       
*
*       Jeremy Batchelder UTLN: jbatch01 
*       Daniel Cetlin UTLN: dcetlin01
*
*       10/20/17
*       compress40.c
*       Purpose: This file is the implementation of the compress40 interface.
*/

#include "headers/compress40.h"
#include "headers/rgb_vcs_converter.h"
#include <pnm.h>
#include "headers/a2methods.h"
#include "headers/a2plain.h"
#include "headers/a2blocked.h"
#include "headers/bit_handler.h"
#include <stdbool.h>
#include <assert.h>
#include <mem.h>

#define A2 A2Methods_UArray2

#define SET_CLOSURE(CLS, FILE) do {                     \
        CLS = ALLOC(sizeof(*CLS));                      \
        assert(CLS != NULL);                            \
        CLS->counter = 0;                               \
        CLS->values = CALLOC(4, sizeof(Vcs));           \
        assert(CLS->values != NULL);                    \
        CLS->fp = FILE;                                 \
} while (0)

/* 
 * trim (used as local function)
 * Purpose: Ensures that the given Pnm_ppm height and width are even.
 *      Trim will correct the height and/or width if either is odd.
 * Parameters: Accepts a Pnm_ppm struct
 * Returns: None
 * CRE: It is a checked runtime error for pmap to be NULL.
 */
void trim(Pnm_ppm pmap) 
{
        if (pmap->height % 2 != 0 ) {
                pmap->height--;
        }
        if (pmap->width % 2 != 0 ) {
                pmap->width--;
        }
}

/* 
 * compress40
 * Purpose: Prints the given pnm image in bits after compressing to stdout
 * Parameters: Accepts a pointer to a file
 * Returns: None
 * CRE: None
 */
extern void compress40  (FILE *input)
{       
        Pnm_ppm pmap = Pnm_ppmread(input, uarray2_methods_plain);
        trim(pmap);

        rgb_to_vcs(pmap);

        Closure cls;
        SET_CLOSURE(cls, NULL);
        
        printf("COMP40 Compressed image format 2\n%u %u\n",
                        pmap->width, pmap->height);

        pmap->methods->small_map_default(pmap->pixels, apply_compress,
                                        (void*) cls);

        FREE((cls->values));
        FREE(cls);

        Pnm_ppmfree(&pmap);
}

/* 
 * decompress40
 * Purpose: Prints the given compressed file in proper pnm format after
 *      decompressing to stdout. 
 * Parameters: Accepts a pointer to a file
 * Returns: None
 * CRE: It is a Checked Runtime Error for there to be fewer than 32 * 
 *      width * height bits or for the header to be incorrect.
 */
extern void decompress40(FILE *input) {
        unsigned height, width;
        int read = fscanf(input, "COMP40 Compressed image format 2\n%u %u", 
                                                &width, &height);
        assert(read == 2);
        int c = getc(input);
        assert(c == '\n');

        A2 array = uarray2_methods_blocked->new_with_blocksize(width, height,
                                                        sizeof(Vcs), 2);

        struct Pnm_ppm pmap = {.width = width, .height = height,
                .denominator = 255, .methods = uarray2_methods_blocked,
                .pixels = array
        };

        Closure cls;
        SET_CLOSURE(cls, input);

        pmap.methods->small_map_default(pmap.pixels, apply_decompress, 
                                        (void*) cls);

        vcs_to_rgb(&pmap);
        Pnm_ppmwrite(stdout, &pmap);

        FREE((cls->values));
        FREE(cls);

        pmap.methods->free(&(pmap.pixels));
}


/* 
 * apply_compress (used as local function)
 * Purpose: Will fill the cl argument with four Vcs structs and convert
 *      these groups of 2*2 pixels into bits      
 * Parameters: cl contains a Closure struct, which has a counter that will 
 *      cycle between 0 and 3, a file pointer that is NULL, and an allocated
 *      array of 4 Vcs structs called values. Also accepts a pointer to an
 *      A2Methods_Object
 * Returns: None
 * CRE: None
 */
void apply_compress(A2Methods_Object *ptr, void *cl) {
        Closure cls = (Closure) cl;
        Vcs pixel = *((Vcs*) ptr);

        cls->values[cls->counter] = pixel;
        cls->counter++;

        if (cls->counter > 3) {
                cls->counter = 0;
                print_bits(convert_to_bits(cls->values));
        }
}

/* 
 * apply_decompress (used as local function)
 * Purpose: If the counter equals 0, apply_decompress
 *      fills the 4 Vcs structs pointed to by cl->values from
 *      the decompressed 32 bit word scanned from the file pointer.
 * Parameters: cl contains a Closure struct, which has a counter that will 
 *      cycle between 0 and 3, a file pointer that is NULL, and an allocated
 *      array of 4 Vcs structs called values. Also accepts a pointer to an
 *      A2Methods_Object
 * Returns: None
 * CRE: None
 */
void apply_decompress(A2Methods_Object *ptr, void *cl)
{
        Closure cls = (Closure) cl;
        if (cls->counter == 0) {
                interpret_bits(cls->values, extract_bits(cls->fp));
        }
        
        *((Vcs*) ptr) = cls->values[cls->counter];

        cls->counter = (cls->counter + 1) % 4;
}