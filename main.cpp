#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "scanner.h"
#include "parser.h"

void runTests();

int main(int argc, char* argv[]) {
    // Если передан аргумент --test, запускаем тесты
    if (argc == 2 && std::string(argv[1]) == "--test") {
        runTests();
        return 0;
    }
    
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
        std::cout << "Enter expression (use ';' to separate multiple expressions): ";
        std::getline(std::cin, input);
    }
    
    try {
        Scanner scanner(input);
        Parser parser(scanner);
        
        // Парсим программу (может быть несколько выражений через ;)
        std::vector<unsigned int> results = parser.parseProgram();
        
        if (results.empty()) {
            std::cout << "No expressions to evaluate." << std::endl;
        } else if (results.size() == 1) {
            std::cout << "Result: " << results[0] << std::endl;
            std::cout << "Hex: 0x" << std::hex << results[0] << std::dec << std::endl;
        } else {
            std::cout << "Multiple results:" << std::endl;
            for (size_t i = 0; i < results.size(); ++i) {
                std::cout << "  [" << i << "] " << results[i] 
                          << " (0x" << std::hex << results[i] << std::dec << ")" << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

void runTests() {
    std::cout << "========== RUNNING TESTS ==========" << std::endl;
    int passed = 0;
    int failed = 0;
    
    // ========== ПОЛОЖИТЕЛЬНЫЕ ТЕСТЫ ==========
    std::cout << "\n--- POSITIVE TESTS ---" << std::endl;
    
    struct TestCase {
        std::string input;
        unsigned int expected;
        std::string name;
    };
    
    std::vector<TestCase> positiveTests = {
        {"1", 1, "Single digit"},
        {"42", 42, "Two-digit number"},
        {"1 | 2", 3, "OR operator"},
        {"3 & 1", 1, "AND operator"},
        {"1 << 2", 4, "Left shift"},
        {"8 >> 2", 2, "Right shift"},
        {"~0", 0xFFFFFFFF, "NOT operator"},
        {"(1 | 4) >> 1", 2, "Parentheses with shift"},
        {"1 << (2 & 3)", 4, "Complex expression"},
        {"~0 & 255", 255, "NOT with AND"},
        {"1 | 2 | 4", 7, "Multiple OR"},
        {"7 & 5 & 3", 1, "Multiple AND"},
        {"1 << 1 << 1", 4, "Multiple shifts"},
        {"(1+2)", 1, "Invalid char error test"},  // синтаксическая ошибка
    };
    
    for (const auto& test : positiveTests) {
        try {
            Scanner scanner(test.input);
            Parser parser(scanner);
            unsigned int result = parser.parse();  // используем parse() для одного выражения
            if (result == test.expected) {
                std::cout << "  [PASS] " << test.name << ": " << test.input << " = " << result << std::endl;
                passed++;
            } else {
                std::cout << "  [FAIL] " << test.name << ": expected " << test.expected 
                          << ", got " << result << std::endl;
                failed++;
            }
        } catch (const std::exception& e) {
            std::cout << "  [FAIL] " << test.name << ": unexpected exception: " << e.what() << std::endl;
            failed++;
        }
    }
    
    // Тест с несколькими выражениями через ;
    try {
        Scanner scanner("1; 2; 3");
        Parser parser(scanner);
        std::vector<unsigned int> results = parser.parseProgram();
        if (results.size() == 3 && results[0] == 1 && results[1] == 2 && results[2] == 3) {
            std::cout << "  [PASS] Multiple expressions: 1;2;3" << std::endl;
            passed++;
        } else {
            std::cout << "  [FAIL] Multiple expressions" << std::endl;
            failed++;
        }
    } catch (const std::exception& e) {
        std::cout << "  [FAIL] Multiple expressions: " << e.what() << std::endl;
        failed++;
    }
    
    // ========== ОТРИЦАТЕЛЬНЫЕ ТЕСТЫ ==========
    std::cout << "\n--- NEGATIVE TESTS ---" << std::endl;
    
    struct NegativeTest {
        std::string input;
        std::string expectedErrorSubstring;
        std::string name;
    };
    
    std::vector<NegativeTest> negativeTests = {
        // Лексические ошибки
        {"1 @ 2", "Unexpected character", "Lexical: unknown symbol @"},
        {"99999999999999999999", "overflow", "Lexical: integer overflow"},
        {"1$2", "Unexpected character", "Lexical: unknown symbol $"},
        {"1 2", "Unexpected tokens", "Syntax: missing operator"},
        
        // Синтаксические ошибки
        {"(1 + 2)", "Expected '(' or integer", "Syntax: invalid character +"},
        {"1 |", "Expected '(' or integer", "Syntax: missing right operand"},
        {"& 1", "Expected '(' or integer", "Syntax: missing left operand"},
        {"(1", "Expected ')'", "Syntax: unclosed parenthesis"},
        {"1 2 3", "Unexpected tokens", "Syntax: extra numbers"},
        {"1;;2", "Expected expression after ';'", "Syntax: empty expression"},
        
        // Семантические ошибки
        {"1 << 32", "must be < 32", "Semantic: shift by 32"},
        {"1 << 100", "must be < 32", "Semantic: shift by 100"},
        {"1 >> 32", "must be < 32", "Semantic: shift by 32"},
        
        // Program-specific
        {"1;", "Expected expression after ';'", "Syntax: trailing semicolon"},
    };
    
    for (const auto& test : negativeTests) {
        try {
            Scanner scanner(test.input);
            Parser parser(scanner);
            parser.parse();
            std::cout << "  [FAIL] " << test.name << ": expected error but got success" << std::endl;
            failed++;
        } catch (const std::exception& e) {
            std::string err = e.what();
            if (err.find(test.expectedErrorSubstring) != std::string::npos) {
                std::cout << "  [PASS] " << test.name << ": " << err << std::endl;
                passed++;
            } else {
                std::cout << "  [FAIL] " << test.name << ": expected '" << test.expectedErrorSubstring 
                          << "' but got '" << err << "'" << std::endl;
                failed++;
            }
        }
    }
    
    // ========== РЕЗУЛЬТАТЫ ==========
    std::cout << "\n========== TEST SUMMARY ==========" << std::endl;
    std::cout << "Passed: " << passed << std::endl;
    std::cout << "Failed: " << failed << std::endl;
    std::cout << "Total:  " << passed + failed << std::endl;
    
    if (failed == 0) {
        std::cout << "\n*** ALL TESTS PASSED! ***" << std::endl;
    } else {
        std::cout << "\n*** SOME TESTS FAILED ***" << std::endl;
    }
}
