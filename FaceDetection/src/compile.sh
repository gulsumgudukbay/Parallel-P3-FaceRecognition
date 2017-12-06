#!/bin/bash
#compile script

gcc -pg -o lbp_seq util.c lbp_seq.c
gcc -pg -o lbp_omp -fopenmp util.c lbp_omp.c
