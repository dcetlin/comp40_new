#ifndef RGB_VCS_CONVERTER_H
#define RGB_VCS_CONVERTER_H

void rgb_to_vcs(Pnm_ppm pmap);

void vcs_to_rgb(Pnm_ppm pmap);

void apply_rgb_to_vcs(int i, int j, A2 array2, A2Methods_Object *ptr, void *cl);

void apply_vcs_to_rgb(int i, int j, A2 array2, A2Methods_Object *ptr, void *cl);

#endif