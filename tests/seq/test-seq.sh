#!/bin/bash
for ((j=1; j<=10; j++)); do
    full=""
    seahorse=""
    elephant=""
    triple_spiral=""
    for ((i=16; i<=8192; i *= 2)); do
        full+=`./mandelbrot_seq -2.5 1.5 -2.0 2.0 $i`
        seahorse+=`./mandelbrot_seq -0.8 -0.7 0.05 0.15 $i`
        elephant+=`./mandelbrot_seq 0.175 0.375 -0.1 0.1 $i`
        triple_spiral+=`./mandelbrot_seq -0.188 -0.012 0.554 0.754 $i`
    done
    echo "$full" >> "full.csv"
    echo "$seahorse" >> "seahorse.csv"
    echo "$elephant" >> "elephant.csv"
    echo "$triple_spiral" >> "triple_spiral.csv"
done