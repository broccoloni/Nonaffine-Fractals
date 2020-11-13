/* Created by:  Liam Graham
 * Last updated: Jun. 2020
 *
 * FILE NAME: fracfuncs.c
 * 
 * This file contains functions that are used to 
 * calculate properties from fractals as well as 
 * manipulate / mutate them for genetic algorithms
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "Fractals.h"
#include "fracfuncs.h"

struct Fractal * makerandfrac(int numpoints, int numfuncs, int *restrictions, int numrestrictions, int disperse, double *window){
    /* This function generates a random fractal. See Fractals.c -> generategenome() for an 
     * explanation of the input parameters
     */
    struct Fractal *frac;
    if ((frac = (struct Fractal *)malloc(sizeof(struct Fractal))) == NULL){
                fprintf(stderr, "Malloc failed. (makerandfrac)\n");
                exit(1);
        }
    initializefrac(frac, numfuncs, numpoints);
    //frac -> coloured = 0;
    generategenome(frac, restrictions, numrestrictions, disperse);
    generatefrac(frac);
    generatematrix(frac, window);
    return frac;
}

void dimension(struct Fractal *frac){
    /* This function calculates an estimate of the
     * fractal dimension for a fractal, based on its
     * corresponding number of pixels, and stores it
     * in the fractal struct.
     */
    frac -> dimension = (log(((double)frac -> numb))/log(((double)WIDTH)));
    return;
}

void stddev(struct Fractal *frac){
    /* This function calculates the standard deviation of the
     * pixels corresponding to a fractal, in both the x and y 
     * direction, based on the image of the fractal. It then
     * stores these values in the fractal struct.
     */
    int i,j;
    double stddevx = 0;
    double stddevy = 0;
    for (i = 0; i < HEIGHT; i++){
        for (j = 0; j < WIDTH; j++){
            if ((frac -> bm[i][j]) == 0){
                stddevx += (j - frac -> avgx) * (j - frac -> avgx);
                stddevy += (i - frac -> avgy) * (i - frac -> avgy);
            }
        }
    }
    frac -> stddevx = sqrt(stddevx/((double)(frac -> numb -1)));
    frac -> stddevy = sqrt(stddevy/((double)(frac -> numb -1)));
    return;
}
