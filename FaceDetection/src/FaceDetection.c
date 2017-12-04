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



int main(void) {
	puts("Hello World!");
	return 0;
}

void create_histogram(int * hist, int ** img, int num_rows, int num_cols)
{
	int cur_sum = 0;
	hist = (int*)malloc(255 * sizeof(int));

	for(int i = 1; i < num_rows-1; i++)
	{
		for(int j = 1; j < num_cols-1; j++)
		{
			cur_sum = 0;
			index = 0;

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


}
