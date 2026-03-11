#include <iostream>
#include <fstream>
#include <string>
#include "scanner.h"
#include "parser.h"

int main(int argc, char* argv[]) {
    std::string input;
    
    if (argc > 1) {
        std::ifstream file(argv[1]);
        if (!file) {
            std::cerr << "Cannot open file: " << argv[1] << std::endl;
            return 1;
        }
        std::string line;
        while (std::getline(file, line)) {
            input += line;
        }
    } else {
        std::cout << "Enter expression: ";
        std::getline(std::cin, input);
    }
    
    try {
        Scanner scanner(input);
        Parser parser(scanner);
        unsigned int result = parser.parse();
        std::cout << "Result: " << result << std::endl;
        std::cout << "Hex: 0x" << std::hex << result << std::dec << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}