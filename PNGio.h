/*Created by:  Liam Graham
 * Date:        Oct. 2018
 * Last updated: Jun. 2020
 *
 * FILE NAME: PNGio.h
 */

struct Fractal;
void funcnumtocolours(int colour, int *r, int *g, int *b);
void WritePNG(char *filename, struct Fractal *frac);
