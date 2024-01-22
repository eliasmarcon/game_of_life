#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <bitset>
#include <cstring>
#include <sstream>
#include <omp.h> // Include the OpenMP header


// Function to print the grid
void printGridParallel(unsigned char* grid, int rows, int cols);

// load board from file
unsigned char* readPatternFromFileParallel(const std::string& filename, int& rows, int& cols);

// write board to file
void writeDataFromGameParallel(const std::string& filename, unsigned char* grid, int& rows, int& cols);

// parallel version of game of life
void gameOfLifeStepParallel(unsigned char* grid, int& generations, int& threads, int& rows, int& cols);