#include "Timing.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "cxxopts.hpp"

// Function to print the grid
void printGrid(const std::vector<std::vector<char>>& grid) {
    for (const auto& row : grid) {
        for (char cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
}

// Function to read the pattern from a file
std::vector<std::vector<char>> readPatternFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file " << filename << std::endl;
        // You might want to handle the error in a way that fits your application
        exit(EXIT_FAILURE);
    }

    std::string line;
    std::getline(file, line);

    // Assuming the first line contains rows and columns separated by a comma
    size_t commaPos = line.find(',');
    int cols = std::stoi(line.substr(0, commaPos));
    int rows = std::stoi(line.substr(commaPos + 1));

    std::vector<std::vector<char>> grid(rows, std::vector<char>(cols, '.'));

    for (int i = 0; i < rows; ++i) {
        std::getline(file, line);
        for (int j = 0; j < cols; ++j) {
            grid[i][j] = line[j];
        }
    }

    file.close();

    return grid;
}


void writeDataFromGame(std::string outputFile, const std::vector<std::vector<char>>& grid) {
    int rows = static_cast<int>(grid.size());
    int cols = (rows > 0) ? static_cast<int>(grid[0].size()) : 0;

    std::ofstream fileStream(outputFile);
    if (!fileStream.is_open()) {
        std::cout << "Output file not found" << std::endl;
        exit(EXIT_FAILURE);
    }

    fileStream << cols << ',' << rows << std::endl;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            fileStream << grid[i][j];
        }
        fileStream << std::endl;
    }

    fileStream.close();
}

// Function to perform one iteration of the Game of Life
void gameOfLifeStep(std::vector<std::vector<char>>& grid) {

    int rows = static_cast<int>(grid.size());
    int cols = static_cast<int>(grid[0].size());
    std::vector<std::vector<char>> newGrid(rows, std::vector<char>(cols, '.'));

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            int liveNeighbors = 0;

            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    int ni = (i + di + rows) % rows;
                    int nj = (j + dj + cols) % cols;

                    liveNeighbors += (grid[ni][nj] == 'x') ? 1 : 0;
                }
            }

            // Exclude the cell itself from the count
            liveNeighbors -= (grid[i][j] == 'x') ? 1 : 0;

            // Apply Game of Life rules
            if (grid[i][j] == 'x' && (liveNeighbors == 2 || liveNeighbors == 3)) {
                newGrid[i][j] = 'x';  // Cell survives
            }
            else if (grid[i][j] == '.' && liveNeighbors == 3) {
                newGrid[i][j] = 'x';  // Cell becomes alive
            }
            else {
                newGrid[i][j] = '.';  // Cell dies
            }
        }
    }

    // Update the original grid
    grid = newGrid;
}

void handleInputArguments(int argc, char* const* argv, std::string *input_filename,
                          std::string *output_filename, int *generations, bool *show_measure)
{

    // REFERENCE https://github.com/jarro2783/cxxopts/tree/master
    cxxopts::Options options("Game of Life 1", "Game of Life 1 Description");

    options.add_options()
        ("load", "<filename> (filename to read input board - Mandatory)", cxxopts::value<std::string>())
        ("save", "<filename> (filename to save output board - Mandatory)", cxxopts::value<std::string>())
        ("generations", "<n> (run n generations)", cxxopts::value<int>()->default_value("5"))
        ("measure", "print time measures (true/t or false/f)", cxxopts::value<std::string>()->default_value("false"))
        ("help", "print usage");

    try
    {
        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            exit(0);
        }

        if (result.count("load"))
            *input_filename = result["load"].as<std::string>();

        if (result.count("save"))
            *output_filename = result["save"].as<std::string>();

        if (result.count("generations"))
            *generations = result["generations"].as<int>();

        /*did this as a string because I wanted to accept true/t and also with bool there where default value issues*/
        if (result["measure"].as<std::string>() == "true" || result["measure"].as<std::string>() == "t") {
            *show_measure = true;
        }
        else if (result["measure"].as<std::string>() == "false" || result["measure"].as<std::string>() == "f"){
            *show_measure = false;
        }


        if (input_filename->empty() || output_filename->empty() || 
            (result["measure"].as<std::string>() != "true" && result["measure"].as<std::string>() != "t" &&
             result["measure"].as<std::string>() != "false" && result["measure"].as<std::string>() != "f")) {

            std::cout << options.help() << std::endl;
            exit(1);
        }
    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error parsing command line options: " << e.what() << std::endl;
        std::cout << options.help() << std::endl;
        exit(1);
    }
}


int main(int argc, char* argv[]) {
    
    Timing* timing = Timing::getInstance();
    std::string input_filename;
    std::string output_filename;
    int generations;
    bool show_measure;

    /*============================================= Initial Step Start ======================================================*/

    // Measure setup time
    timing->startSetup();

    // Handle the input arguments
    handleInputArguments(argc, argv, &input_filename, &output_filename, &generations, &show_measure);

    // Read the pattern from the file
    std::vector<std::vector<char>> pattern = readPatternFromFile(input_filename);

    timing->stopSetup();

    /*============================================= Initial Step End ======================================================*/
    /*
    // Print the initial pattern
    std::cout << "Initial Pattern:" << std::endl;
    printGrid(pattern);
    */
    /*========================================== Computation Start ================================================*/

    // Measure computation time
    timing->startComputation();

    for (int step = 0; step < generations; ++step) {
        gameOfLifeStep(pattern);
    }

    timing->stopComputation();

    /*========================================== Computation End ================================================*/
    /*
    // Print the final pattern
    std::cout << "\nFinal Pattern after " << generations << " iterations:" << std::endl;
    printGrid(pattern);
    std::cout << "\n";
    */
    /*========================================== Finalization Start ================================================*/

    // Measure finalization time
    timing->startFinalization();

    writeDataFromGame(output_filename, pattern);

    timing->stopFinalization();

    // Print timing results
    Timing::getInstance()->print();

    if (show_measure) {
        std::cout << timing->getResults() << std::endl;
    }

    /*========================================== Finalization End ================================================*/

    return 0;
}
