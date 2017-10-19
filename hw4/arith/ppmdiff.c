/*
 *  COMP 40 HW4
 *  Arith
 *
 *  Jeremy Batchelder UTLN: jbatch01
 *  Daniel Cetlin UTLN: dcetli01
 *
 *  10/20/2017
 *  ppmdiff.c
 *  Purpose: Compare two pnm files
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include <pnm.h>
#include <math.h>

typedef A2Methods_UArray2 A2;
typedef A2Methods_mapfun* MapFun;

typedef struct closure{
        double sum;
        int denominator;
        Pnm_ppm pixmap;
} Closure;

double ppmdiff(Pnm_ppm, Pnm_ppm);

void apply_diff (int i, int j, A2 array2, A2Methods_Object *ptr, void *cl);

int main(int argc, char *argv[])
{
        FILE* fp1;
        FILE* fp2;
        
        Pnm_ppm pixmap1, pixmap2;
        int stdin_check = 0;
        
        if (argc != 3) {
                fprintf(stderr,
                        "Usage: ppmdiff [filename, '-'] [filename, '-']\n");
                exit(EXIT_FAILURE);
        }
        if (strcmp(argv[1], "-") == 0) {
                fp1 = stdin;
                stdin_check = 1;
        } else {
                fp1 = fopen(argv[1], "r");
                assert(fp1);
        } 

        pixmap1 = Pnm_ppmread(fp1, uarray2_methods_plain);

        if (strcmp(argv[2], "-") == 0) {
                if (stdin_check) {
                        fprintf(stderr, "Too many standard inputs\n");
                        exit(EXIT_FAILURE);
                }
                fp2 = stdin;
                stdin_check = 1;
        } else {
                fp2 = fopen(argv[2], "r");
                assert(fp2);
        } 

        pixmap2 = Pnm_ppmread(fp2, uarray2_methods_plain);

        if (abs(pixmap1->height - pixmap2->height) > 1
            ||abs(pixmap1->width - pixmap2->width) > 1) {
                fprintf(stderr,
                        "Heights and/or widths differ by more than 1\n");
                printf("1.0\n");
                return EXIT_SUCCESS;
        }

        double d = ppmdiff(pixmap1, pixmap2);
        printf("%.4f\n", d);

        return EXIT_SUCCESS;
}

double ppmdiff(Pnm_ppm pixmap1, Pnm_ppm pixmap2) {
        MapFun map = pixmap1->methods->map_default;

        Closure cls;
        cls.pixmap = pixmap2;
        cls.sum = 0;
        cls.denominator = pixmap1->denominator;

        map(pixmap1->pixels, apply_diff, (void*) &cls);

        return sqrt((cls.sum) / (3 * pixmap1->width * pixmap1->height));
}

void apply_diff (int i, int j, A2 array2, A2Methods_Object *ptr, void *cl)
{
        (void) array2;

        Closure* _cls = ((Closure*) cl);

        Pnm_ppm pixmap2 = _cls->pixmap;
        
        double den1 = _cls->denominator;
        double den2 = pixmap2->denominator;
        
        Pnm_rgb px1 = ((Pnm_rgb) ptr);
        Pnm_rgb px2 = ((Pnm_rgb)(pixmap2->methods->at(pixmap2->pixels, i, j)));

        double r1 = px1->red / den1;
        double g1 = px1->green / den1;
        double b1 = px1->blue / den1;

        double r2 = px2->red / den2;
        double g2 = px2->green / den2;
        double b2 = px2->blue / den2;

        double red_dif = (r1 > r2)? r1 - r2: r2 - r1;
        double green_dif = (g1 > g2)? g1 - g2: g2 - g1;
        double blue_dif = (b1 > b2)? b1 - b2: b2 - b1;

        _cls->sum += (pow((red_dif), 2) + pow((green_dif), 2)
                  + pow((blue_dif), 2));
}