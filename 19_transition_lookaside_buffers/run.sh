#!/bin/bash

NUM_TRIALS=1000000

for i in {1..200..2}
do
    ./tlb $i $NUM_TRIALS
done