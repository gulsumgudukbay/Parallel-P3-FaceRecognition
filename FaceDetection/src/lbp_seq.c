/*
 ============================================================================
 Name        : FaceDetection.c
 Author      : Gulsum Gudukbay
 Version     :
 Copyright   : 
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void create_histogram(int * hist, int ** img, int num_rows, int num_cols);
double distance(int * a, int *b, int size);
int find_closest(int ***training_set, int num_persons, int num_training, int size, int *
test_image);

void create_histogram(int * hist, int ** img, int num_rows, int num_cols)
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

double distance(int * a, int *b, int size)
{
	double sum = 0;
	for(int i = 0; i < size; i++)
	{
		if(a[i] + b[i] != 0)
			sum += 0.5*((a[i]-b[i]) * (a[i]-b[i])) / (a[i]+b[i]);
	}
	return sum;
}



int find_closest(int ***training_set, int num_persons, int num_training, int size, int *
test_image)
{
	int** histograms = (int**)malloc(num_training * num_persons * sizeof(int*));

	for(int i = 0; i < num_training*num_persons; i++)
	{
		histograms[i] = (int*)malloc(size * sizeof(int));
		create_histogram(histograms[i], training_set[i], 182, 202);
	}

	double* distances = (double*)malloc(num_training * num_persons* sizeof(double));

	double min_number = 9999999;
	int min_index = 0;
	for(int i = 0; i < num_training*num_persons; i++)
	{
		distances[i] = distance( histograms[i], test_image, size);
		if(distances[i] < min_number)
		{
			min_number = distances[i];
			min_index = i;
		}
	}

	dealloc_2d_matrix(histograms, num_training * num_persons, size);
	free(distances);

	return min_index / num_training;
}

int main(void) {

	int k = 1; //number of training images for each person
	int n = 18; //number of people
	int p = 20; //number of pictures for each person
	int rows = 182;
	int cols = 202;
	int** closest_indices = alloc_2d_matrix(n, p-k);
	int hist_size = 256;

	//2d arrays of pictures for each picture for each people
	int ****pictures = (int****)malloc(n * sizeof(int***));
	int ***training_sets = (int***)malloc((k * n) * sizeof(int**));

	for(int i = 0; i < n; i++)
	{
		pictures[i] = (int***)malloc(p * sizeof(int**));
	}

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

	for(int i = 1; i <= n; i++)
	{
		for(int j = 1; j <= k; j++)
		{
			filename = malloc( 20 * sizeof(char));
			sprintf(filename, "images/%d.%d.txt", i, j);
			training_sets[k * (i-1) + (j-1)] = read_pgm_file(filename, rows, cols);
			free(filename);
		}
	}

	for(int i = 0; i < n; i++) //traverse each person
	{
		for(int j = k; j < p; j++) //traverse each test image for each person
		{
			int* test_img_hist = (int*)malloc(hist_size * sizeof(int));
			create_histogram(test_img_hist, pictures[i][j], rows, cols);

			//find closest person ids for each person, for each test image
			closest_indices[i][j-k] = find_closest(training_sets, n, k, hist_size, test_img_hist);
			free(test_img_hist);
		}
	}

	int errors = 0;
	for(int i = 0; i < n; i++) //traverse each person
	{
		for(int j = 0; j < p-k; j++) //traverse each test image for each person
		{
			printf("%d.%d.txt\t%d %d\n", i, j+k, closest_indices[i][j], i);
			errors += (closest_indices[i][j] != i);
		}
	}
	printf("Accuracy: %d errors out of %d test images.", errors, n * (p-k));
	dealloc_2d_matrix(closest_indices, n, p-k);

	for(int i = 0; i < (k * n); i++)
		dealloc_2d_matrix(training_sets[i], k*n, p);

	return 0;
}
