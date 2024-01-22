#include "gol_seq.h"

/*===========================================================================================================================================*/
/*========================================================= Helper Functions Sequential =====================================================*/
/*===========================================================================================================================================*/

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


/*===========================================================================================================================================*/
/*=========================================================== Game of Life Sequential =======================================================*/
/*===========================================================================================================================================*/

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

    grid = newGrid;
}