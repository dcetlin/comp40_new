#ifndef COMPRESS40_H
#define COMPRESS40_H

extern void compress40  (FILE *input);  /* reads PPM, writes compressed image */
extern void decompress40(FILE *input);  /* reads compressed image, writes PPM */

#endif