#!/bin/bash
for((k=1; k<=32; k*=2))
do
    echo "16,32,64,128,256,512,1024,2048,4196,8192" > "pth/full/full_$k.csv"
    echo "16,32,64,128,256,512,1024,2048,4196,8192" > "pth/seahorse/seahorse_$k.csv"
    echo "16,32,64,128,256,512,1024,2048,4196,8192" > "pth/elephant/elephant_$k.csv"
    echo "16,32,64,128,256,512,1024,2048,4196,8192" > "pth/triple_spiral/triple_spiral_$k.csv"
    for ((j=1; j<=10; j++)); do
        separator=","
        full=""
        seahorse=""
        elephant=""
        triple_spiral=""
        for ((i=16; i<=8192; i *= 2)); do
            if [ $i -eq 8192 ]
            then
                separator=""
            fi
            full+=`./mandelbrot_pth -2.5 1.5 -2.0 2.0 $i $k`$separator
            seahorse+=`./mandelbrot_pth -0.8 -0.7 0.05 0.15 $i $k`$separator
            elephant+=`./mandelbrot_pth 0.175 0.375 -0.1 0.1 $i $k`$separator
            triple_spiral+=`./mandelbrot_pth -0.188 -0.012 0.554 0.754 $i $k`$separator
        done
        echo "$full" >> "pth/full/full_$k.csv"
        echo "$seahorse" >> "pth/seahorse/seahorse_$k.csv"
        echo "$elephant" >> "pth/elephant/elephant_$k.csv"
        echo "$triple_spiral" >> "pth/triple_spiral/triple_spiral_$k.csv"
    done
done
for((k=1; k<=32; k*=2))
do
    echo "16,32,64,128,256,512,1024,2048,4196,8192" > "omp/full/full_$k.csv"
    echo "16,32,64,128,256,512,1024,2048,4196,8192" > "omp/seahorse/seahorse_$k.csv"
    echo "16,32,64,128,256,512,1024,2048,4196,8192" > "omp/elephant/elephant_$k.csv"
    echo "16,32,64,128,256,512,1024,2048,4196,8192" > "omp/triple_spiral/triple_spiral_$k.csv"
    for ((j=1; j<=10; j++)); do
        separator=","
        full=""
        seahorse=""
        elephant=""
        triple_spiral=""
        for ((i=16; i<=8192; i *= 2)); do
            if [ $i -eq 8192 ]
            then
                separator=""
            fi
            full+=`./mandelbrot_omp -2.5 1.5 -2.0 2.0 $i $k`$separator
            seahorse+=`./mandelbrot_omp -0.8 -0.7 0.05 0.15 $i $k`$separator
            elephant+=`./mandelbrot_omp 0.175 0.375 -0.1 0.1 $i $k`$separator
            triple_spiral+=`./mandelbrot_omp -0.188 -0.012 0.554 0.754 $i $k`$separator
        done
        echo "$full" >> "omp/full/full_$k.csv"
        echo "$seahorse" >> "omp/seahorse/seahorse_$k.csv"
        echo "$elephant" >> "omp/elephant/elephant_$k.csv"
        echo "$triple_spiral" >> "omp/triple_spiral/triple_spiral_$k.csv"
    done
done