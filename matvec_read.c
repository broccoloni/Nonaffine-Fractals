/* This file was created by Allan Willms
 * and is being used with his permission
 *
 * FILE NAME: matvec_read.c
 * 
 * The function in this file reads in a matrix
 * stored in the file filename, and calculates 
 * the number of rows and columns as it does so
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matvec_read.h"

double **matrix_read(char *filename, int *rows, int *cols) {
	/* Read a text file that defines a matrix.
	 * Each row should have the same number of entries as the first row. 
	 * Additional entries are ignored; missing entries are treated as zero.
	 * Entries in a row are separated by whitespace or commas.
	 *
	 * Arguments: 
	 *		input file name,
	 *		pointer to int to store number of rows
	 *		pointer to int to store number of columns
	 *
	 * Returns:  
	 *		NULL pointer on failure
	 *		Pointer to pointer to double otherwise.  The matrix is 
	 *			stored contiguously in memory.
	 */

	int m,n,rowlen;
	double **matrix;
	char b;
	char *line,*tok;
	char delimiters[5] = ", \t\n";
	FILE *infile;

	if ((infile = fopen(filename,"r")) == NULL) {
		fprintf(stderr,"Failed to open file %s.\n",filename);
		return NULL;
	}
	/* First read characters until the first newline character is encountered. */
	rowlen = 0;
	b = fgetc(infile);
	while (b != EOF && b != '\n') {
		rowlen++;
		b = fgetc(infile);
	}
	rowlen += 2;
	rewind(infile);
	/* Allocate space for, and read the first line into a string. */
	line = (char *) malloc(rowlen*sizeof(char));
	fgets(line,rowlen,infile);
	/* Count the number of entries in this line. */
	tok = strtok(line,delimiters);
	*cols = 0;
	while (tok != NULL) {
		(*cols)++;
		tok = strtok(NULL,delimiters);
	}
	free(line);
	/* Count the remaining lines, keeping track of the longest line. */
	*rows = 1;
	n = 0;
	b = fgetc(infile);
	while (b != EOF) {
		if (b == '\n') {
			(*rows)++;
			if (n > rowlen) rowlen = n;
		}
		else
			n++;
		b = fgetc(infile);
	}
	/* Allocate space for longest line. */
	line = (char *) malloc((rowlen+2)*sizeof(char));
	/* Allocate space for the row pointers. */
	if ((matrix = (double **) malloc(*rows*sizeof(double *))) == NULL) {
		fprintf(stderr,"malloc failed\n");
		fclose(infile);
		return NULL;
	}
	/* Rewind file and read lines. */
	rewind(infile);
	/* Allocate space for all rows in contiguous region in memory. */
	if ((matrix[0] = (double *) calloc((*rows)*(*cols),sizeof(double))) == NULL) {
		fprintf(stderr,"calloc failed\n");
		fclose(infile);
		return NULL;
	}
	/* Read each line of file and record values. */
	for (m=0; m<*rows; m++) {
		/* Assign the row pointer to the appropriate location in the allocated space. */
		matrix[m] = matrix[0] + *cols*m;
		/* Get the line from the file. */
		if (fgets(line,rowlen,infile) == NULL) {
			fprintf(stderr,"Error reading input file line %d.\n",m+1);
			fclose(infile);
			return NULL;
		}
		/* Break the line by white space. */
		tok = strtok(line,delimiters);
		for (n=0; n<*cols; n++) {
			if (tok == NULL) break;  /* missing columns treated as zeros */
			if (sscanf(tok,"%lf",matrix[m]+n) != 1) {
				fprintf(stderr,"Error on line %d column %d of input file %s:%s%s.\n",
						m+1,n+1,filename," Trying to read: ",tok);
				fclose(infile);
				return NULL;
			}
			tok = strtok(NULL,delimiters);
		}
	}
	free(line);
	fclose(infile);
	return matrix;
}
