#include "gol_parallel_after_session.h"

// Lookup table for Game of Life rules
const unsigned char rules[2][9] = {
    {0, 0, 0, 1, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 0, 0, 0}
};

/*===========================================================================================================================================*/
/*========================================================== Helper Functions Parallel ======================================================*/
/*===========================================================================================================================================*/

// load board from file in parallel
unsigned char* readPatternFromFileParallelAfterSession(const std::string& filename, int& rows, int& cols) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file " << filename << std::endl;
        // You might want to handle the error in a way that fits your application
        exit(EXIT_FAILURE);
    }

    std::string line;

    // Get first line
    std::getline(file, line);

    // Assuming the first line contains rows and columns separated by a comma
    size_t commaPos = line.find(',');
    cols = std::stoi(line.substr(0, commaPos));
    rows = std::stoi(line.substr(commaPos + 1));

    //sscanf(std::string(line).c_str(), "%d,%d", &rows, &cols);

    unsigned char* grid = new unsigned char[rows * cols]{};

    for (int i = 0; i < rows; ++i) {
        std::getline(file, line);
        for (int j = 0; j < cols; ++j) {
            grid[i * cols + j] = (line[j] == 'x') ? 0x01 : 0x00;
        }
    }

    return grid;
}

// Function to write board to file in parallel
void writeDataFromGameParallelAfterSession(const std::string& outputfile, unsigned char* grid, int& rows, int& cols) {
    std::ofstream fileStream(outputfile);
    if (!fileStream.is_open()) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        exit(EXIT_FAILURE);
    }

    fileStream << cols << ',' << rows << '\n';

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            fileStream << (grid[i * cols + j] == 0x01 ? 'x' : '.');
        }
        fileStream << '\n';
    }

    fileStream.close();
}

/*===========================================================================================================================================*/
/*============================================================ Game of Life Parallel ========================================================*/
/*===========================================================================================================================================*/

// Function to perform one iteration of the Game of Life in parallel using OpenMP
void gameOfLifeStepParallelAfterSession(unsigned char* grid, int& generations, int& threads, int& rows, int& cols) {
    
    unsigned int numLivingNeighbours = 0;
    unsigned char *next_board = (unsigned char *)malloc(rows * cols * sizeof(unsigned char));
    unsigned char *current_board = grid;

    for (int gen = 0; gen < generations; gen++) {
        #pragma omp parallel private(numLivingNeighbours) num_threads(threads)
        {
            #pragma omp for schedule(static)
            for (int y = 0; y < rows; y++) {
                for (int x = 0; x < cols; x++) {
                    numLivingNeighbours = 0;

                    for (int i = -1; i <= 1; i++) {
                        for (int j = -1; j <= 1; j++) {
                            if (i == 0 && j == 0) {
                                continue;
                            }

                            if (current_board[((y + i + rows) % rows) * cols + ((x + j + cols) % cols)] == 0x01) {
                                numLivingNeighbours++;
                            }
                        }
                    }

                    // Apply Game of Life rules using the lookup table
                    next_board[y * cols + x] = rules[current_board[y * cols + x] & 0x01][numLivingNeighbours];
                }
            }
        }

        // Swap pointers instead of using memcpy
        std::swap(current_board, next_board);
    }

    // No need to copy the final result, as current_board is the final state
    // Free the allocated memory for the temporary board
    free(next_board);
}
