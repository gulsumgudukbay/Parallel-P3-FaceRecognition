#!/bin/bash
# run script

rm -f gudukbay_gulsum.output

touch gudukbay_gulsum.output
./lbp_seq 10 >> gudukbay_gulsum.output

echo -e "\n" >> gudukbay_gulsum.output
