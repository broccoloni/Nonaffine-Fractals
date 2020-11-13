/*Created by:  Liam Graham
 * Last updated: Jun. 2020
 *
 * FILE NAME: fracfuncs.h
 */
struct Fractal;
struct Fractal * makerandfrac(int numpoints, int numfuncs, int *restrictions, int numrestrictions, int disperse, double *window);
void dimension(struct Fractal *frac);
void stddev(struct Fractal *frac);
