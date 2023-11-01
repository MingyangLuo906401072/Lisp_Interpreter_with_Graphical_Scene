#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "interpreter.hpp"
#include "expression.hpp"
#include "environment.hpp"
#include "interpreter_semantic_error.hpp"
#include <cstdlib>

int main(int argc, char** argv) {
    // Interpreter init
    Interpreter interpreter;

    try {
        if (argc == 3 && std::string(argv[1]) == "-e") {
            std::string programText = std::string(argv[2]);

            std::istringstream programStream(programText);

            if (interpreter.parse(programStream)) {
                Expression result = interpreter.eval();
                std::cout << "(" << result << ")" << std::endl;
                return EXIT_SUCCESS;
            }
            std::cerr << "Error: Failed to parse the program from the command line." << std::endl;
            return EXIT_FAILURE;
        }
        else if (argc == 2) {
            // Execute program from file
            std::ifstream inputFile(argv[1]);
            if (!inputFile) {
                std::cerr << "Error: Failed to open file: " << argv[1] << std::endl;
                return EXIT_FAILURE;
            }

            if (interpreter.parse(inputFile)) {
                Expression result = interpreter.eval();
                std::cout << "(" << result << ")" << std::endl;
                return EXIT_SUCCESS;
            }
            std::cerr << "Error: Failed to parse the program from the file." << std::endl;
            return EXIT_FAILURE;
        }
        // Interactive mode with REPL
        while (true) {
            std::cout << "slisp> ";
            std::string input;
            std::getline(std::cin, input);
            if (input == "quit" || input == "exit") {
                break; // Exit REPL if "quit" or "exit" is entered
            }
            else if (input.empty()) {
                continue; // Ignore empty lines
            }
            try {
                std::istringstream programStream(input);

                if (interpreter.parse(programStream)) {
                    Expression result = interpreter.eval();
                    std::cout << "(" << result << ")" << std::endl;
                }
                else {
                    std::cerr << "Error: Failed to parse the input." << std::endl;
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}



