# Makefile for compiling the C++ program

# Define the C++ files
cpp_files = main.cpp Timing.cpp ./gol_seq/gol_seq.cpp ./gol_parallel/gol_parallel.cpp ./gol_parallel_after_session/gol_parallel_after_session.cpp

# Define the executable name
executable = ./gol_2

# Compiler and flags
CXX = g++
CXXFLAGS = -fopenmp

# Targets
all: $(executable)

$(executable): $(cpp_files)
	$(CXX) $(CXXFLAGS) -o $@ $(cpp_files)

clean:
	rm -f $(executable)

.PHONY: all clean