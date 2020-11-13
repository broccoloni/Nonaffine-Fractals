/*Created by:  Liam Graham
 * Last updated: Jun. 2020
 *
 * FILE NAME: Fractals.h
 */
#define HEIGHT 640
#define WIDTH 640

struct Fractal{
        double dimension, stddevx, stddevy, *xs, *ys, **genome;
        int fracnum, numfuncs, numpoints, numb, dist, avgx, avgy, **bm, *colours, coloured;
};

double f(double *val, double point, double functype);
int funcind(int funcnum, double **genome);
int funcaddind(int funcnum, double **genome);
int multindjump(int functype);
int addindjump(int functype);
int piecewisecond(double x, double y);
void func(double *x, double *y, double **genome, int funcnum, double functype);
double validranddouble(double functype);
void generatemults(double **genome, double functype, int *multparams);
void generateadds(double **genome, double functype, int *addparams);
void generategenome(struct Fractal *frac, int *restrictions, int numrestrictions, int disperse);
void ordergenome(int numfuncs, double **genome);
double funcdeterminant(double a, double b, double c, double d);
int validatefunc(double a, double b, double c, double d);
double ** mallocgenome(int numfuncs);
void initializefrac(struct Fractal *frac, int numfuncs, int numpoints);
double generatepoints(struct Fractal *frac);
int generatefrac(struct Fractal *frac);
int * pointtocoord(double x, double y, double minx, double maxx, double miny, double maxy);
void generatematrix(struct Fractal *frac, double *window);
void freegenome(struct Fractal *frac);
void freefrac(struct Fractal *frac);
int lenfile(char *filename);








