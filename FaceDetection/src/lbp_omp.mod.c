#ifdef _POMP
#  undef _POMP
#endif
#define _POMP 200110

#include "lbp_omp.c.opari.inc"
#line 1 "lbp_omp.c"
/*
 ============================================================================
 Name        : lbp_omp.c
 Author      : Gulsum Gudukbay
 Description : Face Detection Parallel Code
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include <float.h>
#include <time.h>

void create_histogram(int * hist, int ** img, int num_rows, int num_cols);
double distance(int * a, int *b, int size);
int find_closest(int ***training_set, int num_persons, int num_training, int size, int *
test_image);

void create_histogram(int* hist, int** img, int num_rows, int num_cols)
{
	int cur_sum = 0;

	for(int i = 0; i < 256; i++)
		hist[i] = 0;

	for(int i = 1; i < num_rows-1; i++)
	{
		cur_sum = 0;
		for(int j = 1; j < num_cols-1; j++)
		{
			cur_sum = 0;
			if(img[i][j] < img[i-1][j-1])
				cur_sum += 128;
			if(img[i][j] < img[i-1][j])
				cur_sum += 64;
			if(img[i][j] < img[i-1][j+1])
				cur_sum += 32;
			if(img[i][j] < img[i][j+1])
				cur_sum += 16;
			if(img[i][j] < img[i+1][j+1])
				cur_sum += 8;
			if(img[i][j] < img[i+1][j])
				cur_sum += 4;
			if(img[i][j] < img[i+1][j-1])
				cur_sum += 2;
			if(img[i][j] < img[i][j-1])
				cur_sum += 1;

			hist[cur_sum]++;

		}
	}
}

double distance(int* a, int *b, int size)
{
	double sum = 0;
	for(int i = 0; i < size; i++)
	{
		if(a[i] + b[i] != 0)
			sum += 0.5*(double)((double)(a[i]-b[i]) * (double)(a[i]-b[i])) / (double)(a[i]+b[i]);
	}
	return sum;
}

int find_closest(int ***histograms, int num_persons, int num_training, int size, int *
test_image)
{
	double* distances = (double*)malloc(num_training * num_persons* sizeof(double));

	double min_number = DBL_MAX;
	int min_index = 0;

	for(int i = 0; i < num_training*num_persons; i++)
	{
		distances[i] = distance( (*histograms)[i], test_image, size);
	}

	for(int i = 0; i < num_training*num_persons; i++)
	{
		if(distances[i] < min_number)
		{
			min_number = distances[i];
			min_index = i;
		}
	}
	free(distances);

	return min_index / num_training;
}

int main(int argc, char *argv[]) {
	clock_t begin = clock();

	int k = atoi(argv[1]); //number of training images for each person
	int n = 18; //number of people
	int p = 20; //number of pictures for each person
	int rows = 202;
	int cols = 182;
	int** closest_indices = alloc_2d_matrix(n, p-k);
	int hist_size = 256;

	//2d arrays of pictures for each picture for each people
	int ****pictures = (int****)malloc(n * sizeof(int***));
	int ***training_sets = (int***)malloc((k * n) * sizeof(int**));

POMP_Parallel_fork(&omp_rd_45);
#line 109 "lbp_omp.c"
	#pragma omp parallel     shared(pictures, p)
{ POMP_Parallel_begin(&omp_rd_45);
POMP_For_enter(&omp_rd_45);
#line 109 "lbp_omp.c"
 #pragma omp          for                     nowait
	for(int i = 0; i < n; i++)
		pictures[i] = (int***)malloc(p * sizeof(int**));
POMP_Barrier_enter(&omp_rd_45);
#pragma omp barrier
POMP_Barrier_exit(&omp_rd_45);
POMP_For_exit(&omp_rd_45);
POMP_Parallel_end(&omp_rd_45); }
POMP_Parallel_join(&omp_rd_45);
#line 112 "lbp_omp.c"

	char* filename;

	for(int i = 1; i <= n; i++)
	{
		for(int j = 1; j <= p; j++)
		{
			filename = malloc( 20 * sizeof(char));
			sprintf(filename, "images/%d.%d.txt", i, j);
			pictures[i-1][j-1] = read_pgm_file(filename, rows, cols);
			free(filename);
		}
	}

	int idx = 0;
	for(int i = 1; i <= n; i++)
	{
		for(int j = 1; j <= k; j++)
		{
			filename = malloc( 20 * sizeof(char));
			sprintf(filename, "images/%d.%d.txt", i, j);
			training_sets[idx] = read_pgm_file(filename, rows, cols);
			idx++;
			free(filename);
		}
	}

	//create histograms of training sets
	int** histograms = (int**)malloc(k * n * sizeof(int*));

POMP_Parallel_fork(&omp_rd_46);
#line 142 "lbp_omp.c"
	#pragma omp parallel     shared(histograms, hist_size)
{ POMP_Parallel_begin(&omp_rd_46);
POMP_For_enter(&omp_rd_46);
#line 142 "lbp_omp.c"
 #pragma omp          for                               nowait
	for(int i = 0; i < k*n; i++)
	{
		histograms[i] = (int*)malloc(hist_size * sizeof(int));
	}
POMP_Barrier_enter(&omp_rd_46);
#pragma omp barrier
POMP_Barrier_exit(&omp_rd_46);
POMP_For_exit(&omp_rd_46);
POMP_Parallel_end(&omp_rd_46); }
POMP_Parallel_join(&omp_rd_46);
#line 147 "lbp_omp.c"

POMP_Parallel_fork(&omp_rd_47);
#line 148 "lbp_omp.c"
	#pragma omp parallel     shared(histograms, training_sets, rows, cols)
{ POMP_Parallel_begin(&omp_rd_47);
POMP_For_enter(&omp_rd_47);
#line 148 "lbp_omp.c"
 #pragma omp          for                                               nowait
	for(int i = 0; i < k*n; i++)
	{
		create_histogram(histograms[i], training_sets[i], rows, cols);
	}
POMP_Barrier_enter(&omp_rd_47);
#pragma omp barrier
POMP_Barrier_exit(&omp_rd_47);
POMP_For_exit(&omp_rd_47);
POMP_Parallel_end(&omp_rd_47); }
POMP_Parallel_join(&omp_rd_47);
#line 153 "lbp_omp.c"

	for(int i = 0; i < n; i++) //traverse each person
	{
POMP_Parallel_fork(&omp_rd_48);
#line 156 "lbp_omp.c"
		#pragma omp parallel     shared(pictures, histograms, closest_indices, rows, cols)
{ POMP_Parallel_begin(&omp_rd_48);
POMP_For_enter(&omp_rd_48);
#line 156 "lbp_omp.c"
  #pragma omp          for                                                           nowait
		for(int j = k; j < p; j++) //traverse each test image for each person
		{
			int* test_img_hist = (int*)malloc(hist_size * sizeof(int));
			create_histogram(test_img_hist, pictures[i][j], rows, cols);

			//find closest person ids for each person, for each test image
			closest_indices[i][j-k] = find_closest(&histograms, n, k, hist_size, test_img_hist);
			free(test_img_hist);
		}
POMP_Barrier_enter(&omp_rd_48);
#pragma omp barrier
POMP_Barrier_exit(&omp_rd_48);
POMP_For_exit(&omp_rd_48);
POMP_Parallel_end(&omp_rd_48); }
POMP_Parallel_join(&omp_rd_48);
#line 166 "lbp_omp.c"
	}

	int errors = 0;
	for(int i = 0; i < n; i++) //traverse each person
	{
		for(int j = 0; j < p-k; j++) //traverse each test image for each person
		{
			printf("%d.%d.txt  %d %d\n", i+1, j+k+1, closest_indices[i][j]+1, i+1);
		}
	}

	for(int i = 0; i < n; i++) //traverse each person
	{
POMP_Parallel_fork(&omp_rd_49);
#line 179 "lbp_omp.c"
		#pragma omp parallel     reduction(+:errors)
{ POMP_Parallel_begin(&omp_rd_49);
POMP_For_enter(&omp_rd_49);
#line 179 "lbp_omp.c"
  #pragma omp          for                     nowait
		for(int j = 0; j < p-k; j++) //traverse each test image for each person
		{
			errors += (closest_indices[i][j] != i);
		}
POMP_Barrier_enter(&omp_rd_49);
#pragma omp barrier
POMP_Barrier_exit(&omp_rd_49);
POMP_For_exit(&omp_rd_49);
POMP_Parallel_end(&omp_rd_49); }
POMP_Parallel_join(&omp_rd_49);
#line 184 "lbp_omp.c"
	}
	printf("Accuracy: %d errors out of %d test images.\n", errors, n * (p-k));

	dealloc_2d_matrix(histograms, k * n, hist_size);
	dealloc_2d_matrix(closest_indices, n, p-k);

POMP_Parallel_fork(&omp_rd_50);
#line 190 "lbp_omp.c"
	#pragma omp parallel     shared(training_sets, p, k, n)
{ POMP_Parallel_begin(&omp_rd_50);
POMP_For_enter(&omp_rd_50);
#line 190 "lbp_omp.c"
 #pragma omp          for                                nowait
	for(int i = 0; i < (k * n); i++)
		dealloc_2d_matrix(training_sets[i], k*n, p);
POMP_Barrier_enter(&omp_rd_50);
#pragma omp barrier
POMP_Barrier_exit(&omp_rd_50);
POMP_For_exit(&omp_rd_50);
POMP_Parallel_end(&omp_rd_50); }
POMP_Parallel_join(&omp_rd_50);
#line 193 "lbp_omp.c"

POMP_Parallel_fork(&omp_rd_51);
#line 194 "lbp_omp.c"
	#pragma omp parallel     shared(pictures, rows, cols)
{ POMP_Parallel_begin(&omp_rd_51);
POMP_For_enter(&omp_rd_51);
#line 194 "lbp_omp.c"
 #pragma omp          for                              nowait
	for(int i = 0; i < n; i++)
		for(int j = 0; j < p; j++)
			dealloc_2d_matrix(pictures[i][j], rows, cols);
POMP_Barrier_enter(&omp_rd_51);
#pragma omp barrier
POMP_Barrier_exit(&omp_rd_51);
POMP_For_exit(&omp_rd_51);
POMP_Parallel_end(&omp_rd_51); }
POMP_Parallel_join(&omp_rd_51);
#line 198 "lbp_omp.c"

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Parallel time: %f\n", time_spent);
	return 0;
}
