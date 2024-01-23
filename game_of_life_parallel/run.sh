#!/bin/bash

# # Check for the number of tasks and diameter arguments
# if [ "$#" -gt 2 ]; then
#     echo "Usage: $0 [<mode>] [<num_runs>]"
#     exit 1
# fi

# # Set default values if arguments are not provided
# mode=${1:-"seq"}
# num_runs=${2:-"2"}
mode="$MODE"
num_runs="$ITERATIONS"


# Define the C++ file to be compiled and executed
cpp_file="main.cpp Timing.cpp ./gol_seq/gol_seq.cpp ./gol_parallel/gol_parallel.cpp"
executable="./gol_2"
save_file_1="./output_files/ai23m020"
outputfile="./dumb.txt"

# Compile the C++ file
#g++ -fopenmp -o $executable $cpp_file
make all

# Array to store the fastest computation time
fastest_computation_times=()

echo "$mode"

if [ "$mode" == "seq" ]; then
    csv_file="./csv_files/ai23m020_cpu_time.csv"
    save_file_2="cpu_out.txt"
elif [ "$mode" == "omp" ]; then
    csv_file="./csv_files/ai23m020_openmp_time.csv"
    save_file_2="openmp_out.txt"
elif [ "$mode" == "omp_after_session" ]; then
    csv_file="./csv_files/ai23m020_openmp_after_session_time.csv"
    save_file_2="openmp_after_session_out.txt"
else
    echo "Invalid mode: $mode. Exiting..."
    exit 1
fi

echo -e "$csv_file\n"

# iterate size
min_iterations=1000
max_iterations=10000

for ((current_run_number = min_iterations; current_run_number <= max_iterations; current_run_number += 1000)); do
    
    # Define file names
    load_file="../step1000_in_250generations/random${current_run_number}_in.gol"
    save_file="${save_file_1}_${current_run_number}_${save_file_2}"

    # Run the C++ program multiple times and append measurements to the CSV file
    # for ((i = 1; i <= num_runs; i++)); do
    #     echo "Running iteration $i for current_run_number $current_run_number..."
    #     if [ "$mode" == "seq" ]; then
    #         $executable --mode "$mode" --load "$load_file" --save "$save_file" --generations 250 --measure t >> "$outputfile"
    #     elif [ "$mode" == "omp" ]; then
    #         $executable --mode "$mode" --threads 16 --load "$load_file" --save "$save_file" --generations 250 --measure t >> "$outputfile"
    #     fi
    # done

    for ((i = 1; i <= num_runs; i++)); do
        echo "Running iteration $i for current_run_number $current_run_number..."
        $executable --mode "$mode" --load "$load_file" --save "$save_file" --generations 250 --measure t >> "$outputfile"
    done

    # Read the header line from the CSV file
    header_line=$(head -n 1 "$csv_file")

    # Find the fastest element based on the second element (computation time)
    fastest_computation_time=$(tail -n +2 "$csv_file" | awk -F ';' '{print $2}' | sort -n | head -n 1)

    #echo "Fastest computation time $fastest_computation_time for $current_run_number"
    #mapfile -t all_elements < <(tail -n +2 "$csv_file")
    #for element in "${all_elements[@]}"; do
    #    echo "$element"
    #done

    # Find and save the corresponding fastest element(s)
    fastest_element=$(grep "$fastest_computation_time" "$csv_file")

    fastest_computation_times+=("$fastest_element")

    # Clear the CSV file for the next iteration, keeping the header line
    echo "$header_line" > "$csv_file"
done

# Print the array of fastest computation times
echo -e "\nFastest computation time(s) for each iteration:"
for fastest_time in "${fastest_computation_times[@]}"; do
    echo "$fastest_time"

    # Write the fastest time back into the CSV file, appending to the existing content
    echo "$fastest_time" >> "$csv_file"
done

# run checking script
python3 output_check.py "$mode"

rm $outputfile