#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void dstrtovec(char *str, double *vector, int *len){
	char *ptr;
	*len = 0;
	
	ptr = strtok(str, ",");
	while (ptr != NULL) {
		vector[*len] = atof(ptr);
		ptr = strtok(NULL, ",");
		*len += 1;
	}
}

void istrtovec(char *str, int *vector, int *len){
        char *ptr;
        *len = 0;

        ptr = strtok(str, ",");
        while (ptr != NULL) {
                vector[*len] = atof(ptr);
                ptr = strtok(NULL, ",");
                *len += 1;
        }
}

double ** dmatmem(int m, int n){
	int i;
	double **A;
	A = (double **)malloc(m * sizeof(double *));
	for (i=0;i<m;i++){
		A[i] = (double *)malloc(n * sizeof(double));
	}
	return A;
}

int * ivecmem(int m) {
        int *vec;
        vec = (int *)malloc(m * sizeof(int));
        return vec;
}

double * dvecmem(int m) {
        double *vec;
        vec = (double *)malloc(m * sizeof(double));
        return vec;
}

void dvecout(int n, double *vec){
	for (int i = 0; i < n; i++){
		fprintf(stdout, "%.4lf\t", vec[i]);
	}
	fprintf(stdout, "\n");
}

/* Sorting function for ordergenome in Fractals.c */
void dsortmatrows(int m, int col, double **mat){
	/* Sorts a matrix by from smallest to largest in column col */
	int i, swapped, n;
	double *tmp;
	for (i = 0; i < m; i++){
		swapped = 0;
		n = 0;
		while (n < (m-1)){		
			if (mat[n][col] > mat[n+1][col]){
				tmp = mat[n];
				mat[n] = mat[n+1];
				mat[n+1] = tmp;
				swapped = 1;
			}
			n += 1;
		}
		if (swapped == 0) break;
	}
	return;
}

void dsortvec(int m, double *vec){
        /* Sorts a list from lowest to heighest */
        int i, swapped, n;
        double tmp;
        for (i = 0; i < m; i++){
                swapped = 0;
                n = 0;
                while (n < (m - 1)){
                        if (vec[n] > vec[n+1]){
                                tmp = vec[n];
                                vec[n] = vec[n+1];
                                vec[n+1] = tmp;
                                swapped = 1;
                        }
			n += 1;
		}
		if (swapped == 0) break;
        }
        return;
}

void dfreemat(int m, double **mat){
	for (int i = 0; i < m; i++){
		free(mat[i]);
	}
	free(mat);
}

void ifreemat(int m, int **mat){
	for (int i = 0; i < m; i++){
		free(mat[i]);
	}
	free(mat);
}


