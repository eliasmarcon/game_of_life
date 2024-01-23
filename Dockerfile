# Use an official Ubuntu runtime as a parent image
FROM ubuntu:22.04

# Set non-interactive frontend (useful for Docker builds)
ENV DEBIAN_FRONTEND noninteractive

# Install necessary dependencies
RUN apt-get update -y && \
    apt-get install -y \
    g++ \
    libomp-dev \
    libstdc++-9-dev \
    cmake \
    python3 \
    python3-pip \
    dos2unix

# Copy the current directory contents into the container at /app
COPY . /app

# Copy the current directory contents into the container at /app
WORKDIR /app/game_of_life_parallel

# Convert line endings in the run.sh script
RUN dos2unix run.sh && \
    chmod +x run.sh


# Run your application
CMD ["bash", "run.sh", "MODE", "ITERATIONS"]