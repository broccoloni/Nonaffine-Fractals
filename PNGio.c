/* Created by:  Liam Graham
 * Last updated: Jun. 2020
 *
 * FILE NAME PNGio.c
 *
 * This file contains functions that are used to 
 * for fractal image input and output. Specifically,
 * to convert pixel maps of fractals to a png, either
 * coloured or not
 */

#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <math.h>
#include "PNGio.h"
#include "Fractals.h"

void funcnumtocolours(int colour, int *r, int *g, int *b){
    /* This function is used to convert a function number
     * to a specific colour so that you can visualize which
     * function made what part of a fractal
     *
     * function  0:    red
     *           1:    orange
     *           2:    yellow
     *           3:    green
     *           4:    blue
     * add more colours if you want to colour an attractor
     * that has more than 4 functions in its IFS
     */

    if (colour == 0){
        *r = 255;
        *g = 0;
        *b = 0;
    }
    else if (colour == 1){
        *r = 255;
        *g = 128;
        *b = 0;
    }
    else if (colour == 2){
        *r = 255;
        *g = 255;
        *b = 0;
    }
    else if (colour == 3){
        *r = 0;
        *g = 255;
        *b = 0;
    }
    else if (colour == 4){
        *r = 0;
        *g = 128;
        *b = 255;
    }
    return;
}

void WritePNG(char *filename, struct Fractal *frac){
    /* This function converts a pixel map of a fractal,
     * stored in the fractal structure, to a png image
     * stored in the given filename.
     *
     * The fractal structure also contains an integer 
     * named coloured, if coloured is 0, the fractal is 
     * coloured based on which function output what point
     * according to the colours assigned in funcnumtocolours.
     * If coloured is 1 then the fractal is black.
     */
    FILE *fp = fopen(filename, "wb");
    if (!fp) abort();
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) abort();
    png_infop info = png_create_info_struct(png);
    if (!info) abort();
    if (setjmp(png_jmpbuf(png))) abort();
    png_init_io(png, fp);
    png_set_IHDR(
        png, 
        info, 
        WIDTH, 
        HEIGHT, 
        8, 
        PNG_COLOR_TYPE_RGB, 
        PNG_INTERLACE_NONE, 
        PNG_COMPRESSION_TYPE_DEFAULT, 
        PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info); 
    png_bytep *row_pointers = (png_bytep *)malloc(HEIGHT * sizeof(png_bytep));
    for (int i = 0; i < HEIGHT; i++){
        row_pointers[i] = (png_bytep)malloc(3 * WIDTH * sizeof(unsigned char));
    }
    int r,g,b;
    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            if (frac -> bm[i][j] != 255 && frac -> coloured == 0){
                funcnumtocolours(frac -> bm[i][j], &r, &g, &b);
                row_pointers[i][3*j+0] = (unsigned char) r;
                row_pointers[i][3*j+1] = (unsigned char) g;
                row_pointers[i][3*j+2] = (unsigned char) b;
            }
            else if (frac -> bm[i][j] != 255 && frac -> coloured == 1){
                row_pointers[i][3*j+0] = (unsigned char) 0;
                                row_pointers[i][3*j+1] = (unsigned char) 0;
                                row_pointers[i][3*j+2] = (unsigned char) 0;
            }
            else {
                row_pointers[i][3*j+0] = (unsigned char) 255;
                row_pointers[i][3*j+1] = (unsigned char) 255;
                row_pointers[i][3*j+2] = (unsigned char) 255;
            }
        }
    }
    png_write_image(png, row_pointers);
    png_write_end(png, NULL);
    fclose(fp);
    if (png && info) png_destroy_write_struct(&png, &info);
    return;
}
