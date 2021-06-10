#!/bin/bash
echo "16,32,64,128,256,512,1024,2048,4196,8192" >> "full.csv"
echo "16,32,64,128,256,512,1024,2048,4196,8192" >> "seahorse.csv"
echo "16,32,64,128,256,512,1024,2048,4196,8192" >> "elephant.csv"
echo "16,32,64,128,256,512,1024,2048,4196,8192" >> "triple_spiral.csv"
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
        full+=`./mandelbrot_seq -2.5 1.5 -2.0 2.0 $i`$separator
        seahorse+=`./mandelbrot_seq -0.8 -0.7 0.05 0.15 $i`$separator
        elephant+=`./mandelbrot_seq 0.175 0.375 -0.1 0.1 $i`$separator
        triple_spiral+=`./mandelbrot_seq -0.188 -0.012 0.554 0.754 $i`$separator
    done
    
    echo "$full" >> "full.csv"
    echo "$seahorse" >> "seahorse.csv"
    echo "$elephant" >> "elephant.csv"
    echo "$triple_spiral" >> "triple_spiral.csv"
done