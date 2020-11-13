/*Created by:    Liam Graham
 * Last updated: Jun. 2020
 *
 * FILE NAME: generatedata.c
 * 
 * This is the main file that, when run,
 * generates databases of fractals
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "Fractals.h"
#include "vecio.h"
#include "PNGio.h"
#include "fracfuncs.h"

int main(int argc, char *argv[]){
    int i, j, numpoints, numfuncs, numrows, numtogenerate, numrestrictions, disperse, tmpint;
    int *restrictions = ivecmem(20);
    int pcomp = 0; 
    double window[4];
    char filename[50], dirname[50], fracname[124],filepath[100],tmp[50];
    FILE *fp;
    struct Fractal *frac;
    srand(time(NULL));
    
    fprintf(stdout, "How many fractals would you like to generate: ");
    scanf("%d", &numtogenerate);
    fprintf(stdout, "\n");
    sprintf(filename, "fracdata.dat");
    fprintf(stdout, "What is the directory called (Note: it should already be created): ");
    scanf("%s", dirname);
    fprintf(stdout, "\nHow many points would you like to plot for each fractal: ");
    scanf("%d", &numpoints);
    strcpy(filepath, dirname); 
    fprintf(stdout, "\nHow many functions in each IFS: ");
    scanf("%d", &numfuncs);
    fprintf(stdout, "\nEnter a vector representing the viewing window (eg. minx,maxx,miny,maxy): ");
    scanf("%s", tmp);
    dstrtovec(tmp, window, &tmpint);
    fprintf(stdout, "\n0  - Affine\n"); 
    fprintf(stdout, "1  - x -> acos(bx) + ccos(dy)+e\n");
    fprintf(stdout, "2  - x -> acos(bx) + csin(dy)+e\n");
    fprintf(stdout, "3  - x -> acos(bx) + ctanh(dy)+e\n");
    fprintf(stdout, "4  - x -> asin(bx) + ccos(dy)+e\n");
    fprintf(stdout, "5  - x -> asin(bx) + csin(dy)+e\n");
    fprintf(stdout, "6  - x -> asin(bx) + ctanh(dy)+e\n");
    fprintf(stdout, "7  - x -> atanh(bx) + ccos(dy)+e\n");
    fprintf(stdout, "8  - x -> atanh(bx) + csin(dy)+e\n");
    fprintf(stdout, "9  - x -> atanh(bx) + ctanh(dy)+e\n");
    fprintf(stdout, "10 - Piecewise Affine\n");
    fprintf(stdout, "\nEnter a vector containing the maps you would like to restrict: ");
    scanf("%s", filepath); //filepath is just a temporary placeholder
    fprintf(stdout, "\n");
    istrtovec(filepath, restrictions, &numrestrictions);
    fprintf(stdout, "\n0 - No restrictions\n");
    fprintf(stdout, "1 - Ensure there are at least 2 different transformation types\n");
    fprintf(stdout, "2 - Ensure there is at least 1 of each transformation type\n");
    fprintf(stdout, "\nWhat dispersion of transformations would you like: ");
    scanf("%d", &disperse);
    fprintf(stdout, "\n");
    sprintf(filepath, "%s%s", dirname, filename);
    if ((fp = fopen(filepath, "r")) == NULL){
        numrows = 0;
    }
    else {
        numrows = lenfile(filepath);
    }
    if ((fp = fopen(filepath, "a")) == NULL){
        fprintf(stderr, "Error, you must create the directory first\n");
        exit(1);
    }
    fprintf(stdout, "Generating fractals %d to %d\n", numrows, numrows+numtogenerate);
    for (i = 0; i < numtogenerate; i++){
        frac = makerandfrac(numpoints, numfuncs, restrictions, numrestrictions, disperse, window);
        stddev(frac);
        dimension(frac);
        int params = 0;
        /* fractal number, numfuncs, numpoints, numb, avgx, avgy, stddevx, stddevy, dimension, genome */
        fprintf(fp, "%d\t%d\t%d\t%d\t%d\t%d\t%.15lf\t%.15lf\t%.15lf\t",
                numrows+i, frac->numfuncs, frac->numpoints, frac->numb, 
                frac->avgx, frac->avgy, frac->stddevx, frac->stddevy, frac -> dimension);
        for (j = 0; j < frac -> numfuncs; j++){
            for (int k = 0; k < multindjump(frac->genome[3][j]); k++){
                fprintf(fp, "%.15lf\t", frac -> genome[0][params + k]);
            }
            params += multindjump(frac->genome[3][j]);
        }
        params = 0;
        for (j = 0; j < frac -> numfuncs; j++){
            for (int k = 0; k < addindjump(frac->genome[3][j]); k++){
                fprintf(fp, "%.15lf\t", frac -> genome[1][params + k]);
            }
            params += addindjump(frac->genome[3][j]);
        }
        for (j = 0; j < frac -> numfuncs; j++){
            fprintf(fp, "%.15lf\t", frac -> genome[2][j]);
        }
        for (j = 0; j < frac -> numfuncs-1; j++){
            fprintf(fp, "%.15lf\t", frac -> genome[3][j]);
        }
        fprintf(fp, "%.15lf\n", frac -> genome[3][frac -> numfuncs -1]);
        sprintf(fracname, "%sfrac%d.png", dirname, numrows+i);
        WritePNG(fracname, frac);
        freefrac(frac);
        if (numtogenerate >= 100 && (i%((int)(numtogenerate/100.)) == 0)){
            pcomp += 1;
            if (pcomp < 10){
                fprintf(stdout, "\rPercent Complete:\t%d%%", pcomp);
            }
            else if (pcomp < 100){
                fprintf(stdout, "\rPercent Complete:\t\b%d%%", pcomp);
            }
            else {
                fprintf(stdout, "\rPercent Complete:\t\b\b%d%%",pcomp);
            }
            fflush(stdout);
        }
    }
    fprintf(stdout, "\n"); 
    fclose(fp);
    exit(0);
}



