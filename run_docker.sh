#!/bin/bash

# Check for the number of arguments
if [ "$#" -eq 2 ]; then
    mode=$1
    iterations=$2
else
    echo "Usage: ./run_docker.sh <mode> <iterations>"
    exit 1
fi

modes=("seq" "omp" "omp_after_session")

for mode in "${modes[@]}"; do
    
    echo -e "Running script with mode: $mode\n"

    echo -e "\nBuilding the Docker image...\n"
    # Build the docker image
    docker build --build-arg MODE="$mode" --build-arg ITERATIONS="$iterations" -t game_of_life .

    echo -e "\nRunning the Docker container...\n"
    # Run the docker container
    docker run -e MODE="$mode" -e ITERATIONS="$iterations" -it game_of_life

    echo -e "\nCopying the output files from the Docker container to the host...\n"
    # Get the Docker container ID
    DOCKER_ID=$(docker ps -lq)
    # Copy the output file from the Docker container to the host

    if [ "$mode" == "seq" ]; then
        docker cp "$DOCKER_ID:/app/game_of_life_parallel/csv_files/ai23m020_cpu_time.csv" ./game_of_life_parallel/csv_files/ai23m020_cpu_time.csv
    elif [ "$mode" == "omp" ]; then
        docker cp "$DOCKER_ID:/app/game_of_life_parallel/csv_files/ai23m020_openmp_time.csv" ./game_of_life_parallel/csv_files/ai23m020_openmp_time.csv
    elif [ "$mode" == "omp_after_session" ]; then
        docker cp "$DOCKER_ID:/app/game_of_life_parallel/csv_files/ai23m020_openmp_after_session_time.csv" ./game_of_life_parallel/csv_files/ai23m020_openmp_after_session_time.csv
    else
        echo "Invalid mode"
        exit 1
    fi

    echo -e "\nDeleting the Docker container..."
    # Delete the Docker container
    docker rm "$DOCKER_ID"
done