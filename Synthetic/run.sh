#!/bin/sh
clear
clear

GCC_files="*.cpp"

input="../Inputs/input.txt"
output="../Outputs/output"
sweeps=100
heading=270

cd "Java_code"
java Main $input $output $sweeps $heading

cd "../C_code"
echo "Compiling C++ code..."
for file in $GCC_files
do
  echo "Compiling $file..."
  g++ "$file"
done

echo "Converting to NMEA sentences..."
./a.out

echo "Converting NMEA sentences into NMEA file.."
