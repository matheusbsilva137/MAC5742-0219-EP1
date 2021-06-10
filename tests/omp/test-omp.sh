#!/bin/bash
for((k=1; k<=32; k*=2)): do
    echo "16,32,64,128,256,512,1024,2048,4196,8192" >> "/full/full_$k.csv"
    echo "16,32,64,128,256,512,1024,2048,4196,8192" >> "/seahorse/seahorse_$k.csv"
    echo "16,32,64,128,256,512,1024,2048,4196,8192" >> "/elephant/elephant_$k.csv"
    echo "16,32,64,128,256,512,1024,2048,4196,8192" >> "/triple_spiral/triple_spiral_$k.csv"
    for ((j=1; j<=10; j++)); do
        full=""
        seahorse=""
        elephant=""
        triple_spiral=""
        for ((i=16; i<=8192; i *= 2)); do
            full+=`./mandelbrot_omp -2.5 1.5 -2.0 2.0 $i $k`
            seahorse+=`./mandelbrot_omp -0.8 -0.7 0.05 0.15 $i $k`
            elephant+=`./mandelbrot_omp 0.175 0.375 -0.1 0.1 $i $k`
            triple_spiral+=`./mandelbrot_omp -0.188 -0.012 0.554 0.754 $i $k`
        done
        echo "$full" >> "/full/full_$k.csv"
        echo "$seahorse" >> "/seahorse/seahorse_$k.csv"
        echo "$elephant" >> "/elephant/elephant_$k.csv"
        echo "$triple_spiral" >> "/triple_spiral/triple_spiral_$k.csv"
    done
done
