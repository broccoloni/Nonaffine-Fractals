/*Created by:  Liam Graham
 * Last updated: Jun. 2020
 *
 * FILE NAME: Fractals.c
 *
 * This file contains functions that are used to 
 * generate fractals, both affine and non-affine
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include "Fractals.h"
#include "vecio.h"
#include "matvec_read.h"
#define DOTSIZE 1 //must be an odd positive integer

double f(double *val, double point, double functype){
    /* This function is used to compute non-affine transformations
     * on a single coordinate point. (ie. only x for the point (x, y)) 
     * where the transformation type is represented as an integer, 
     * functype, computed with a randomly generated value, val, 
     * stored in the IFS genome
     */
    double newval;
    if (functype == 1){
        newval = val[0]*tanh(val[1]*point);
    }
    if (functype == 2){
        newval = val[0]*sin(val[1]*point);
    }
    if (functype == 3){
        newval = val[0]*tanh(val[1]*point);
    }
    return newval;
}

int funcind(int funcnum, double **genome){
    /* This function returns the starting index
     * of funcnumth (like nth) function in the 
     * fractals genome. This function is calculates
     * the starting index of the multiplicative
     * parameters 
     */
    int i;
    int ind = 0;
    int j;
    for (i = 0; i < funcnum; i++){
        j = genome[3][i];
        if (j == 0) ind += 4;
        else if (j<11) ind += 8;
    }
    return ind;
}

int funcaddind(int funcnum, double **genome){
    /* This function returns the starting index
     * of funcnumth (like nth) function in the 
     * fractals genome. This function is calculates
     * the starting index of the additive
     * parameters 
     */
    int i;
    int ind = 0;
    int j;
    for (i = 0; i < funcnum; i++){
        j = genome[3][i];
        if (j < 10) ind += 2;
        else ind += 4;
    }
    return ind;
}

int multindjump(int functype){
    /* This function is similar to funcind */
    if (functype == 0) return 4;
    else if (functype < 11) return 8;
    else return 0;
}

int addindjump(int functype){
    /* This function is similar to funcindadd */
    if (functype < 10) return 2;
    else if (functype == 10) return 4;
    else return 0;
}

int piecewisecond(double x, double y){
    /* This function is used for piecewise affine IFSs
     * It calculates whether a point is on one side of
     * the piecewise boundary,  or the other, returning
     * either 0 or 1
     * With the current if statement the piecewise 
     * boundary is the circle of radius 1/2 centered at 0
     */
    if (fabs(x) + fabs(y) < 0.5) return 1;
    else return 0;

}

void func(double *x, double *y, double **genome, int funcnum, double functype){
    /* This function computes the transformation of a point (x, y) by a 
     * function in an IFS. The specific function in the IFS is given by
     * an integer, funcnum, then the parameters of that function are obtained 
     * from the IFS genome. The function type is then used to compute the 
     * transformation. 
     *
     * if functype is 0:    affine transformation   new_x = ax + by + c
     *                1:    2 to 1 trig mapping     new_x = acos(bx) + ccos(dy) + e
     *                2:    2 to 1 trig mapping     new_x = acos(by) + csin(dy) + e
     *                3:    2 to 1 trig mapping     new_x = acos(by) + ctanh(dy) + e
     *                4:    2 to 1 trig mapping     new_x = asin(bx) + ccos(dy) + e
     *                5:    2 to 1 trig mapping     new_x = asin(bx) + csin(dy) + e
     *                6:    2 to 1 trig mapping     new_x = asin(bx) + ctanh(dy) + e
     *                7:    2 to 1 trig mapping     new_x = atanh(bx) + ccos(dy) + e
     *                8:    2 to 1 trig mapping     new_x = atanh(bx) + csin(dy) + e
     *                9:    2 to 1 trig mapping     new_x = atanh(bx) + ctanh(dy) + e
     *                10:   piecewise affine        new_x = {ax+by + c, x < 0
     *                                                       dx+ey + f, x >= 0
     *
     */
    double oldx = *x;
    double oldy = *y;
    int ind = funcind(funcnum, genome);
    int addind = funcaddind(funcnum, genome);
    if (functype == 0){
        (*x) = genome[0][ind]   * oldx + genome[0][ind+1] * oldy + genome[1][addind];
        (*y) = genome[0][ind+2] * oldx + genome[0][ind+3] * oldy + genome[1][addind+1];
    }
    else if (functype == 1){
        (*x) = f(&(genome[0][ind]), oldx, 1) + f(&(genome[0][ind+2]), oldy, 1) + genome[1][funcnum*2];
        (*y) = f(&(genome[0][ind+4]), oldx, 1) + f(&(genome[0][ind+6]), oldy, 1) + genome[1][funcnum*2+1];
    }
    else if (functype == 2){
        (*x) = f(&(genome[0][ind]), oldx, 1) + f(&(genome[0][ind+2]), oldy, 2) + genome[1][funcnum*2];
        (*y) = f(&(genome[0][ind+4]), oldx, 1) + f(&(genome[0][ind+6]), oldy, 2) + genome[1][funcnum*2+1];
    }
    else if (functype == 3){
        (*x) = f(&(genome[0][ind]), oldx, 1) + f(&(genome[0][ind+2]), oldy, 3) + genome[1][funcnum*2];
        (*y) = f(&(genome[0][ind+4]), oldx, 1) + f(&(genome[0][ind+6]), oldy, 3) + genome[1][funcnum*2+1];
    }
    else if (functype == 4){
        (*x) = f(&(genome[0][ind]), oldx, 2) + f(&(genome[0][ind+2]), oldy, 1) + genome[1][funcnum*2];
        (*y) = f(&(genome[0][ind+4]), oldx, 2) + f(&(genome[0][ind+6]), oldy, 1) + genome[1][funcnum*2+1];
    }
    else if (functype == 5){
        (*x) = f(&(genome[0][ind]), oldx, 2) + f(&(genome[0][ind+2]), oldy, 2) + genome[1][funcnum*2];
        (*y) = f(&(genome[0][ind+4]), oldx, 2) + f(&(genome[0][ind+6]), oldy, 2) + genome[1][funcnum*2+1];
    }
    else if (functype == 6){
        (*x) = f(&(genome[0][ind]), oldx, 2) + f(&(genome[0][ind+2]), oldy, 3) + genome[1][funcnum*2];
        (*y) = f(&(genome[0][ind+4]), oldx, 2) + f(&(genome[0][ind+6]), oldy, 3) + genome[1][funcnum*2+1];
    }
    else if (functype == 7){
        (*x) = f(&(genome[0][ind]), oldx, 3) + f(&(genome[0][ind+2]), oldy, 1) + genome[1][funcnum*2];
        (*y) = f(&(genome[0][ind+4]), oldx, 3) + f(&(genome[0][ind+6]), oldy, 1) + genome[1][funcnum*2+1];
    }
    else if (functype == 8){
        (*x) = f(&(genome[0][ind]), oldx, 3) + f(&(genome[0][ind+2]), oldy, 2) + genome[1][funcnum*2];
        (*y) = f(&(genome[0][ind+4]), oldx, 3) + f(&(genome[0][ind+6]), oldy, 2) + genome[1][funcnum*2+1];
    }
    else if (functype == 9){
        (*x) = f(&(genome[0][ind]), oldx, 3) + f(&(genome[0][ind+2]), oldy, 3) + genome[1][funcnum*2];
        (*y) = f(&(genome[0][ind+4]), oldx, 3) + f(&(genome[0][ind+6]), oldy, 3) + genome[1][funcnum*2+1];
    }
    else if (functype == 10){
        if (piecewisecond(oldx, oldy)) {
            (*x) = genome[0][ind]   * oldx + genome[0][ind+1] * oldy + genome[1][addind];
            (*y) = genome[0][ind+2] * oldx + genome[0][ind+3] * oldy + genome[1][addind+1];
        }
        else {        
            (*x) = genome[0][ind+4] * oldx + genome[0][ind+5] * oldy + genome[1][addind+2];
            (*y) = genome[0][ind+6] * oldx + genome[0][ind+7] * oldy + genome[1][addind+3];
        }
    }
}

double validranddouble(double functype){
    /* This function generates random values
     * within a specific range for IFS parameters
     */
    double min;
    double range;
    // for most parameters:
    if (functype >= 0) {
        min = -1.;
        range = 2.;
    }
    //for the b values in bounded derivative functions
    else if (functype == -1){
        min = 1.;
        range = 4.;
    }
    double val = (double)rand()/RAND_MAX*range + min;
    if (functype == -1){
        min = (double)rand()/RAND_MAX; 
        if (min < 0.5) val *= -1.;
    }
    return val;
}

void generatemults(double **genome, double functype, int *multparams){
    /* This function generates the multiplicative parameters 
     * of each function in an IFS. ie., the parameters that are not
     * the +c or +e in the functions defined in the func() function
     *
     * The parameters are generated using validranddouble function
     * and the function is checked if it satisfies contractivity
     * conditions. If the function does not satisfy the contractivity
     * conditions, all parameters for that function are regenerated.
     */
    int numparams, i;
    int pass = 1;
    if (functype == 0) numparams = 4;
    else  numparams = 8;
    
    i = *multparams;
    if (functype == 0){
        while (pass != 0){
            for (int j = i; j < i + 4; j++){
                genome[0][j] = validranddouble(functype);
            }
            pass = validatefunc(genome[0][i], genome[0][i+1], genome[0][i+2], genome[0][i+3]);
        }
        pass = 1;
    }
    else if (functype < 10){
        while (pass != 0){
            for (int j = 0; j < 4; j++){
                genome[0][i + 2*j] = validranddouble(functype);
            }
            pass = validatefunc(genome[0][i], genome[0][i+2], genome[0][i+4], genome[0][i+6]);
        }
        for (int j = 0; j < 4; j++){
            genome[0][i + 2*j+1] = validranddouble(-1);
        }
        pass = 1;
    }
    else if (functype == 10){
        while (pass != 0){
            for (int j = i; j < i + 4; j++){
                genome[0][j] = validranddouble(functype);
            }
            pass = validatefunc(genome[0][i], genome[0][i+1], genome[0][i+2], genome[0][i+3]);
        }
        pass = 1;
        while (pass != 0){
            for (int j = i+4; j < i + 8; j++){
                genome[0][j] = validranddouble(functype);
            }
            pass = validatefunc(genome[0][i+4], genome[0][i+5], genome[0][i+6], genome[0][i+7]);
        }
        pass = 1;
    }
    *multparams += numparams;
    return;
}

void generateadds(double **genome, double functype, int *addparams){
    /* This function generates the additive parameters
     * for each function in the IFS. ie., the +c or +e in the 
     * functions defined in the func() function.
     *
     * Since these parameters do not affect contractivity,
     * they are generated to be a random number between 
     * -1 and 1.
     */
    int i;
    int numparams;
    if (functype < 10) numparams = 2;
    else numparams = 4;

    for (i = *addparams; i < *addparams + numparams; i++){
        genome[1][i] = (double)rand()/RAND_MAX*2. - 1.;
    }
    *addparams += numparams;
    return;
}

void generategenome(struct Fractal *frac, int *restrictions, int numrestrictions, int disperse){
    /* This function generates the genome of a fractal, ie., the set of vectors that  
     * that contains all the parameters used to generate the fractal. 
     * NOTE: This function assumes that initializefrac has been called
     *
     * INPUTS:
     *      frac            - the fractal struct for which the genome is being 
     *                        generated and stored in 
     *      restrictions    - a vector containing function types for which the 
     *                        fractal can't have based on the function types 
     *                        defined in the func() function. Eg., if restrictions
     *                        were the vector {2,3,4} then the fractal can have 
     *                        functypes {0,1,5,6,7,8,9,10}
     *      numrestrictions - an integer corresponding to the length of the 
     *                        restrictions vector.
     *      disperse        - an integer defining the dispersion of the functions 
     *                        according to the restrictions.
     *      if disperse is 0: there are no restrictions, function types are randomly 
     *                        generated so long as they're not in the restrictions
     *                        vector. There is no guaruntee that a specific function 
     *                        type will be present in the resulting fractal.
     *                     1: this option requires at least 2 different types of
     *                        functions. However, if there are more than 2 allowed 
     *                        types of functions then it is not guarunteed to contain
     *                        each type.
     *                     2: this option will enforce that at least one of each 
     *                        function type will be in the resulting fractal.
     */
    int i, j;
    int multparams = 0;
    int addparams = 0;
    int pass = 1;
    int premade = 0;
    int numfunctypes = 11;
    double **genome = frac -> genome;
    if (disperse == 1) {
        premade = 2;
        for (i = 0; i < 2; i++){
            while (pass != 0){
                pass = 0;
                genome[3][i] = rand()%numfunctypes;
                for (j = 0; j < numrestrictions; j++){
                    if (genome[3][i] == restrictions[j]) pass = 1;
                    if ((i == 1) && (genome[3][i] == genome[3][i-1])) pass = 1;
                }
            }
        }
    }
    if (disperse == 2) {
        for (i = 0; i < numfunctypes; i++){
            pass = 0;
            for (j = 0; j < numrestrictions; j++){
                if (i == restrictions[j]) pass = 1;
            }
            if (pass == 0) {
                genome[3][premade] = i;
                premade++;
            }
        }
        pass = 1;
    }
    for (i = premade; i < frac -> numfuncs; i++){
        while (pass != 0){ 
            pass = 0;
            genome[3][i] = rand()%numfunctypes;
            for (j = 0; j < numrestrictions; j++){
                if (genome[3][i] == restrictions[j]) pass = 1;
            }
        }
        pass = 1;
    }
    dsortvec(frac -> numfuncs, genome[3]);
    for (i = 0; i < frac -> numfuncs; i++){
        generatemults(genome, genome[3][i], &multparams);
        generateadds(genome, genome[3][i], &addparams);
        genome[2][i] = 1./(double)frac -> numfuncs;
    }
    return;
}

void ordergenome(int numfuncs, double **genome){
    /* This function is used to order the functions in the 
    * fractal genome. This is done by sorting rows of a
    * matrix where each row represents a function
    */
    int i,j, curind, indf;
    double **genomefuncs = dmatmem(numfuncs, 22);
    /* Setting values in the genomefuncs matrix */
    for (i = 0; i < numfuncs; i++){
        genomefuncs[i][0] = genome[3][i]; //place functype first
        curind = 1;
        indf = funcind(i, genome);
        for (j = 0; j < multindjump(genome[3][i]); j++){
            genomefuncs[i][curind] = genome[0][indf + j];
            curind++;
        }
        indf = funcaddind(i, genome);
        for (j = 0; j < addindjump(genome[3][i]); j++){
            genomefuncs[i][curind] = genome[1][indf + j];
            curind++;
        }
        genomefuncs[i][curind] = genome[2][i];
    }
    dsortmatrows(numfuncs, 4, 1, genomefuncs);

    /* Converting back to genome of the fractal */
    for (i = 0; i < numfuncs; i++){
        genome[3][i] = genomefuncs[i][0];
    }
    for (i = 0; i < numfuncs; i++){
        curind = 1;
        indf = funcind(i, genome);
        for (j = 0; j < multindjump(genomefuncs[i][0]); j++){
            genome[0][indf + j] = genomefuncs[i][curind];
            curind++;
        }
        indf = funcaddind(i, genome);
        for (j = 0; j < addindjump(genomefuncs[i][0]); j++){
            genome[1][indf + j] = genomefuncs[i][curind];
            curind++;
        }
        genome[2][i] = genomefuncs[i][curind];
    }
    return;
}

double funcdeterminant(double a, double b, double c, double d){
    /* This function is used to calculate the determinant
     * of an affine transformation matrix in order to 
     * check contractivity.
     */
    return a*d - c*b;
}

int validatefunc(double a, double b, double c, double d){
    /* This function checks if an affine transformation
     * satisfies contractivity conditions. This function
     * returns 0 if contractive, 1 otherwise.
     *
     * if an affine transformation is given by:
     *
     *       [x]   [a b][x]   [e]
     *       [y] = [c d][y] + [f]
     *
     * then these contractivity conditions are
     *  a^2 + c^2 < 1
     *  b^2 + d^2 < 1
     *  a^2 + b^2 + c^2 + d^2 - det(A)^2 < 1
     */
    double val1, val2;
    double det = funcdeterminant(a,b,c,d);
    if ((val1 = a*a + c*c)>=1) return 1;
    else if ((val2 = b*b + d*d)>=1) return 1;
    else if ((val1 + val2 - det*det) >= 1) return 1;
    else return 0;
}

double ** mallocgenome(int numfuncs){
    /* This function allocates memory for the genome of an IFS 
     * The genome consists of 4 vectors
     * genome[0] is the vector of multiplicative parameters
     * genome[1] is the vector of additive parameters
     * genome[2] is the vector of probabilities for each function
     * genome[3] is the vector of functypes
     */
    double **genome;
    if ((genome = (double **)malloc(4*sizeof(double *))) == NULL){
        fprintf(stderr, "Malloc failed (generate genome)\n");
        exit(1);
    }
    if (((genome[0] = (double *)malloc(8*numfuncs*sizeof(double))) == NULL)||
        ((genome[1] = (double *)malloc(4*numfuncs*sizeof(double))) == NULL)||
        ((genome[2] = (double *)malloc(numfuncs*sizeof(double))) == NULL)){
            fprintf(stderr, "Malloc failed (initializefrac)\n");
            exit(1);
    }
    if ((genome[3] = (double *)malloc(numfuncs*sizeof(double))) == NULL){
        fprintf(stderr, "Malloc failed (initializefrac 3)\n");
        exit(1);
    }
    return genome;
}

void initializefrac(struct Fractal *frac, int numfuncs, int numpoints){
    /* This function initializes a fractal structure. The number of
     * points and number of functions has to be defined before it 
     * can be called. This function then allocates memory for the
     * x and y points that will be generated as well as allocating 
     * memory for the matrix representing the picture of the fractal.
     *
     * All values corresponding to the fractal other than numfuncs,
     * numpoints, and whether the fractal is colours or not
     * are initialized to -1
     */
    int i;
    frac -> fracnum   = -1;
    frac -> numfuncs  = numfuncs;
    frac -> numpoints = numpoints;
    frac -> numb      = -1;
    frac -> avgx      = -1;
    frac -> avgy      = -1;
    frac -> stddevx   = -1;
    frac -> stddevy   = -1;
    frac -> dimension = -1;
    frac -> dist      = -1;
    frac -> coloured  = 1; //dont colour fractals by function by default
                           //to make them coloured by function by default
                           //change this to 0

    /* initialize genome */
    double **genome = mallocgenome(numfuncs);
    frac -> genome = genome;

    /* initialize xs, ys, and colour vector*/
    if ((frac -> xs = (double *)malloc(numpoints * sizeof(double))) == NULL){
        fprintf(stderr, "Malloc Failed. (initialize points)\n");
        exit(1);
    }
    if ((frac -> ys = (double *)malloc(numpoints * sizeof(double))) == NULL){
        fprintf(stderr, "Malloc Failed. (initialize points)\n");
        exit(1);
    }
    if ((frac -> colours = (int *)malloc(numpoints * sizeof(int))) == NULL){
        fprintf(stderr, "Malloc Failed. (initialize points)\n");
        exit(1);
    }
    /* initizlize the pixel map */
    int **bm;
    if ((bm = (int **)malloc(HEIGHT*sizeof(int *))) == NULL){
        fprintf(stdout, "Malloc Failed. (makematrix)\n");
        exit(1);
    }
    for (i = 0; i < HEIGHT; i++){
        if ((bm[i] = (int *)malloc(WIDTH*sizeof(int))) == NULL){
            fprintf(stderr, "Malloc Failed. (makematrix)\n");
            exit(1);
        }
    }
    frac -> bm = bm;
    return;
}

double generatepoints(struct Fractal *frac){
    /* This function generates the points corresponding 
     * to a fractal. That is, it randomly picks a function
     * in the fractal and uses it to transform a random 
     * point. A new function is then picked and transforms 
     * the output from the last point. This continues until
     * numpoints points are generated. Additionally, the 
     * first 100 points are thrown away to ensure that all 
     * (or close to all) points correspond to the fractal. 
     */
    int i,j;
    int funcnum;
    double p, num;
    double max = 0;
    double x = (double)rand()/RAND_MAX; 
    double y = (double)rand()/RAND_MAX;
    double maxx = 0;
    double maxy = 0;
    for (i = 0; i < 100; i++){
        funcnum = rand()%frac -> numfuncs;
        func(&x, &y, frac -> genome, funcnum, frac -> genome[3][funcnum]);
    }
    for (i = 0; i < frac -> numpoints; i++){
        num = (double)rand()/RAND_MAX;
        p = 0.0;
        funcnum = 0;
        for (j = 0; j < frac -> numfuncs; j++){
            p += frac -> genome[2][j];
            if (num < p) {
                func(&x,&y,frac -> genome, funcnum, frac -> genome[3][j]);
                frac -> xs[i] = x;
                frac -> ys[i] = y;
                
                //note: colours get put to pixels in generatebm (below)
                //      and colours chosen are in PNGio.c
                frac -> colours[i] = funcnum;
                if (fabs(x) > max) max = fabs(x);
                if (fabs(y) > max) max = fabs(y);
                if (fabs(x) > maxx) maxx = fabs(x);
                if (fabs(y) > maxy) maxy = fabs(y);
                break;
            }
            else funcnum ++;
        }
    }
    return max;
}

int generatefrac(struct Fractal *frac){
    /* This function calls the generate points function.
     * The commented section is used to resized the affine
     * fractals if they were too large to fit into the -1 to 1 
     * square, however, this is no longer used as non-affine
     * IFSs are not as simple to resize
     */
    double max = generatepoints(frac);
    int resized = 0;    
    /*
    int i = 0;
    if (max > 1.0){
        for (int i = 0; i < 2 * frac -> numfuncs; i++){
            frac -> genome[1][i] /= max;
        }
        max = generatepoints(frac);
        resized = 1;
        i++;
    }
    */
    max += 1; // so the compiler doesn't give a warning for not using max
    return resized;
}

int * pointtocoord(double x, double y, double minx, double maxx, double miny, double maxy){
    /* This function is used to convert a point (x,y) to pixel coordinates
     * on a screen with viewing region [minx,maxx]x[miny,maxy]
     */
    int *coords = ivecmem(2);
    coords[0] = (int)(WIDTH/2  + WIDTH/2  * ((x - minx)/(maxx - minx)*2 - 1));
    coords[1] = (int)(HEIGHT/2 - HEIGHT/2 * ((y - miny)/(maxy - miny)*2 - 1));
    return coords;
}

void generatematrix(struct Fractal *frac, double *window){
    /* This function is used to transform the points of a fractal
     * to a matrix of size HEIGHT x WIDTH which will be used to
     * generate an image of the fractal.
     */
    int i,j,k,x,y;
    int dotsize = DOTSIZE; //positive odd integer - defines the size of a point
    int white = 255;
    int **bm = frac -> bm;
    
    //start off with a fully white image
    for (i = 0; i < HEIGHT; i++){
        for (j = 0; j < WIDTH; j++){
            bm[i][j] = white;
        }
    }
    //numb is the number of pixels corresponding to the attractor
    //avgx and avgy are the pixel centroid coordinates
    int numb = 0;
    int avgx = 0;
    int avgy = 0;
    int *coords;
    for (i = 0; i < frac -> numpoints; i++){
	coords = pointtocoord(frac ->xs[i], frac->ys[i], window[0], 
                              window[1], window[2], window[3]);
        x = coords[0];
        y = coords[1];
        if (dotsize %2 != 0) {
             for (j = -1 * (dotsize -1)/2; j <= (dotsize - 1)/2; j++){
                 for (k = -1 * (dotsize -1)/2; k <= (dotsize -1)/2; k++){
                     if (x >= WIDTH  - dotsize/2 - 1) x = WIDTH  - dotsize/2 - 1;
                     if (y >= HEIGHT - dotsize/2 - 1) y = HEIGHT - dotsize/2 - 1;
                     if (x <= dotsize/2) x = dotsize;
                     if (y <= dotsize/2) y = dotsize;
                     if (bm[y+j][x+k] == 255){
                         avgx += x+k;
                         avgy += y+j;
                         numb += 1;
                     }
                     bm[y+j][x+k] = frac -> colours[i];
                }
            }
        }
    }
    frac -> avgx = avgx/(int)numb;
    frac -> avgy = avgy/(int)numb;
    frac -> numb = numb;
    return;
}

void freegenome(struct Fractal *frac){
    /* This function frees the genome memory */
    free(frac -> genome[0]);
    free(frac -> genome[1]);
    free(frac -> genome[2]);
    free(frac -> genome[3]);
    free(frac -> genome);
    return;
}

void freefrac(struct Fractal *frac){
    /* This function frees the memory of a fractal structure */
    for (int i = 0; i < HEIGHT; i++){
        free(frac -> bm[i]);
    }
    free(frac -> bm);
    freegenome(frac);
    free(frac -> xs);
    free(frac -> ys);
    return;
}

int lenfile(char *filename){
    /* Thise function calculates the length of a file */
    int len = 0;
    char ch;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL){
        fprintf(stderr, "Failed to open file (lenfile): %s (%c)\n", filename, 'r');
        exit(1);
    }
    while (!feof(fp)){
        ch = fgetc(fp);
        if (ch == '\n'){
            len++;
        }
    }
    fclose(fp);
    return len;
}
