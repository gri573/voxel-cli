#!/bin/bash
cd worldgen
echo "Entering directory \"worldgen\"."
for F in *.c
do
echo "Compiling $F"
gcc -c $F
done
echo "Leaving directory \"worldgen\"."
cd ../interact
echo "Entering directory \"interact\"."
for F in *.c
do
echo "Compiling $F"
gcc -c $F
done
echo "Leaving directory \"interact\"."
cd ../rendering
echo "Entering directory \"rendering\"."
for F in *.c
do
echo "Compiling $F"
gcc -c $F
done
echo "Leaving directory \"rendering\"."
cd ..
echo "Entering root directory"
for F in *.c
do
echo "Compiling $F"
gcc -c $F
done
gcc $(find . -name "*.o") -o voxel_cli
