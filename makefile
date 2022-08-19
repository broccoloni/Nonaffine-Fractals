all:	
	gcc -Wall -o generatedata generatedata.c Fractals.c fracfuncs.c PNGio.c vecio.c matvec_read.c -lm -lpng
