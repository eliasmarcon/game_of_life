#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "cxxopts.hpp"

#include "Timing.h"
#include "./gol_seq/gol_seq.h"
#include "./gol_parallel/gol_parallel.h"

/*===========================================================================================================================================*/
/*============================================================== Helper Functions ===========================================================*/
/*===========================================================================================================================================*/

// Function to append timing results to a CSV file
void appendToCsv(const std::string& filename, const std::string& results) {
    std::ofstream fileStream;

    // Open the file in append mode
    fileStream.open(filename, std::ios::app);

    if (!fileStream.is_open()) {
        std::cerr << "Error opening file " << filename << " for appending" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Write the results to the file
    fileStream << results << std::endl;

    // Close the file
    fileStream.close();
}


void handleInputArguments(int argc, char* const* argv, std::string *mode, int *threads, std::string *input_filename,
                          std::string *output_filename, int *generations, bool *show_measure)
{

    // REFERENCE https://github.com/jarro2783/cxxopts/tree/master
    cxxopts::Options options("Game of Life 2", "Game of Life 2 Description");

    options.add_options()
        ("mode", "<seq | omp> Mode sequential or parallel", cxxopts::value<std::string>())
        ("threads", "<NUM> (parallel OpenMP version with NUM threads)", cxxopts::value<int>())
        ("load", "<filename> (filename to read input board - Mandatory)", cxxopts::value<std::string>())
        ("save", "<filename> (filename to save output board - Mandatory)", cxxopts::value<std::string>())
        ("generations", "<n> (run n generations)", cxxopts::value<int>()->default_value("5"))
        ("measure", "<True | true | t | False | false | f> print time measures", cxxopts::value<std::string>()->default_value("false"))
        ("help", "print usage");

    try
    {
        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            exit(0);
        }

        if (result.count("mode"))
            *mode = result["mode"].as<std::string>();

        if (result["mode"].as<std::string>() == "omp")
        {
            if (result.count("threads"))
                *threads = result["threads"].as<int>();
            else
                *threads = omp_get_max_threads();
        }

        if (result.count("load"))
            *input_filename = result["load"].as<std::string>();

        if (result.count("save"))
            *output_filename = result["save"].as<std::string>();

        if (result.count("generations"))
            *generations = result["generations"].as<int>();

        /*did this as a string because I wanted to accept true/t and also with bool there where default value issues*/
        if (result["measure"].as<std::string>() == "True" || result["measure"].as<std::string>() == "true" || result["measure"].as<std::string>() == "t") {
            *show_measure = true;
        }
        else if (result["measure"].as<std::string>() == "False" || result["measure"].as<std::string>() == "false" || result["measure"].as<std::string>() == "f"){
            *show_measure = false;
        }


        if (input_filename->empty() || output_filename->empty() || (result["mode"].as<std::string>() != "seq" && result["mode"].as<std::string>() != "omp") ||
            (result["measure"].as<std::string>() != "True" && result["measure"].as<std::string>() != "true" && result["measure"].as<std::string>() != "t" &&
             result["measure"].as<std::string>() != "False" && result["measure"].as<std::string>() != "false" && result["measure"].as<std::string>() != "f")) {

            std::cout << options.help() << std::endl;
            exit(1);
        }
    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error parsing command line options: " << e.what() << std::endl;
        std::cout << options.help() << std::endl;
        exit(1);
    }
}

/*===========================================================================================================================================*/
/*====================================================================== main ===============================================================*/
/*===========================================================================================================================================*/

int main(int argc, char* argv[]) {
    
    Timing* timing = Timing::getInstance();
    std::string mode;
    int threads;
    std::string input_filename;
    std::string output_filename;
    int generations;
    bool show_measure;

    // sequential version
    std::vector<std::vector<char>> pattern_seq;
    
    // parallel version
    unsigned char* pattern_par;
    int rows, cols;

    /*============================================= Initial Step Start ====================================================*/

    // Measure setup time
    timing->startSetup();

    // Handle the input arguments
    handleInputArguments(argc, argv, &mode, &threads, &input_filename, &output_filename, &generations, &show_measure);

    // Read the pattern from the file
    if (mode == "seq")
        pattern_seq = readPatternFromFile(input_filename);
    else if (mode == "omp")
        pattern_par = readPatternFromFileParallel(input_filename, rows, cols);

    timing->stopSetup();

    /*============================================= Initial Step End ======================================================*/

    /*============================================= Computation Start =====================================================*/

    // Measure computation time
    timing->startComputation();

    if (mode == "seq"){
        for (int step = 0; step < generations; ++step) {
            gameOfLifeStep(pattern_seq);
        }
    }
    else if (mode == "omp"){
        
        gameOfLifeStepParallel(pattern_par, generations, threads, rows, cols);
    }

    timing->stopComputation();

    /*============================================= Computation End ======================================================*/
 
    /*============================================ Finalization Start ====================================================*/

    // Measure finalization time
    timing->startFinalization();

    if (mode == "seq")
        writeDataFromGame(output_filename, pattern_seq);
    else if (mode == "omp")
        writeDataFromGameParallel(output_filename, pattern_par, rows, cols);

    timing->stopFinalization();

    // Print timing results
    Timing::getInstance()->print();

    if (show_measure) {
        
        std::string results = timing->getResults();
        std::cout << results << std::endl;

        std::string csvFilename = "";
        std::string folderName = "./csv_files/";

        // Save results to CSV file
        if (!results.empty()) {

            if (mode == "seq")
                csvFilename = folderName + "ai23m020_cpu_time.csv";
            else if (mode == "omp")
                csvFilename = folderName + "ai23m020_openmp_time.csv";

            // Check if the CSV file already exists
            std::ifstream file(csvFilename);
            bool fileExists = file.good();
            file.close();

            // Append or create the CSV file
            if (fileExists) {
                appendToCsv(csvFilename, results);
            } else {
            
                std::ofstream csvStream(csvFilename);
                if (csvStream.is_open()) {
                    csvStream << "setup;computation;finalization" << std::endl;
                    csvStream << results << std::endl;
                    csvStream.close();
                } else {
                    std::cerr << "Error creating CSV file: " << csvFilename << std::endl;
                }
            }
        }
    }

    /*=========================================== Finalization End ====================================================*/

    return 0;
}