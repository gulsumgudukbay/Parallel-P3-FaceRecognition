#!/bin/bash
# run script

rm -f prof_sequential.txt
touch prof_sequential.txt
rm -f prof_omp.txt
touch prof_omp.txt
rm -f gudukbay_gulsum.output
touch gudukbay_gulsum.output

k_values=(1 2 5 7 10)
num_threads_values=(1 2 4 6 8 16)

echo -e "Sequential Outputs:\n" >> gudukbay_gulsum.output

for (( i=0; i<5; i++ ))
do
  echo -e "k=${k_values[$i]}\n" >> gudukbay_gulsum.output
  ./lbp_seq ${k_values[$i]} >> gudukbay_gulsum.output
  echo -e "\n" >> gudukbay_gulsum.output
  gprof lbp_seq gmon.out > prof_sequential.txt
  echo -e "\n" >> prof_sequential.txt
done


echo -e "Parallel Outputs:\n" >> gudukbay_gulsum.output

for(( j = 0; j < 6; j++))
do
  export OMP_NUM_THREADS=${num_threads_values[$j]}
  echo -e "num_of_threads=${num_threads_values[$j]}\n" >> gudukbay_gulsum.output
  for (( i=0; i<5; i++ ))
  do
    echo -e "k=${k_values[$i]}\n" >> gudukbay_gulsum.output
    ./lbp_omp ${k_values[$i]} >> gudukbay_gulsum.output
    echo -e "\n" >> gudukbay_gulsum.output
    gprof lbp_omp gmon.out >> prof_omp.txt
    echo -e "\n" >> prof_omp.txt
  done
done
