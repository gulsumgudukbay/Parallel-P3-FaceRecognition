#ifdef _POMP
#  undef _POMP
#endif
#define _POMP 200110

#include "util.c.opari.inc"
#line 1 "util.c"
#include "util.h"

int **alloc_2d_matrix(int r, int c)
{
	int ** a;
	int i;
	a = (int **)malloc(sizeof(int *) * r);
	if (a == NULL) {
		perror("memory allocation failure");
		exit(0);
	}
	for (i = 0; i < r; ++i) {
		a[i] = (int *)malloc(sizeof(int) * c);
		if (a[i] == NULL) {
			perror("memory allocation failure");
			exit(EXIT_FAILURE);
		}
	}
	return a;
}

void dealloc_2d_matrix(int **a, int r, int c)
{
	int i;
	for (i = 0; i < r; ++i)
		free(a[i]);
	free(a);
}

int ** read_pgm_file(char * file_name, int h, int w)
{
	FILE * file;
	int i,j, int_tmp;
	int** data=alloc_2d_matrix(h,w);

	for(int i = 0; i < h; i++)
		for(int j = 0; j < w; j++)
			data[i][j] = 0;

	if ((file = fopen(file_name, "r+")) == NULL)
	{
		printf("ERROR: file open failed\n");
		return(NULL);
	}

	for (i = 1; i < (h); i++)
	{
		for (j = 1; j < (w); j++)
		{
			fscanf(file,"%d", &int_tmp);
			data[i][j] = int_tmp;
		}
	}
	fclose(file);
	return data;
}