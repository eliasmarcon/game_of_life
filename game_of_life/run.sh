#!/bin/bash

# Define the C++ file to be compiled and executed
cpp_file="main.cpp Timing.cpp"
executable="./gol"
save_file_1="./output_files/ai23m020"
save_file_2="cpu_out.txt"
outputfile="./dumb.txt"

# Compile the C++ file
g++ -fopenmp -o $executable $cpp_file

num_runs=1  # Adjust this to the desired number of runs
min_iterations=1000
max_iterations=10000

for ((current_run_number = min_iterations; current_run_number <= max_iterations; current_run_number += 1000)); do
    
    # Define file names
    load_file="../step1000_in_250generations/random${current_run_number}_in.gol"
    save_file="${save_file_1}_${current_run_number}_${save_file_2}"

    # Run the C++ program multiple times and append measurements to the CSV file
    for ((i = 1; i <= num_runs; i++)); do
        echo "Running iteration $i for current_run_number $current_run_number..."

        $executable --load "$load_file" --save "$save_file" --generations 250 --measure t >> "$outputfile"

    done

done


rm $outputfile