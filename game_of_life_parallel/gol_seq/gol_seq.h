#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Function to print the grid
void printGrid(const std::vector<std::vector<char>>& grid);

// Function to read the pattern from a file
std::vector<std::vector<char>> readPatternFromFile(const std::string& filename);

// Function to write data from the game to a file
void writeDataFromGame(std::string outputFile, const std::vector<std::vector<char>>& grid);

// Function to perform one iteration of the Game of Life
void gameOfLifeStep(std::vector<std::vector<char>>& grid);