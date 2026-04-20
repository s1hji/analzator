#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include <stdexcept>
#include <vector>

class Parser {
public:
    Parser(Scanner& scanner);
    unsigned int parse();          // парсит одно выражение (для обратной совместимости)
    std::vector<unsigned int> parseProgram();  // парсит программу из нескольких выражений
    
private:
    Scanner& scanner;
    Token currentToken;
    
    void consume(TokenType expected);
    unsigned int parseProgramPrime();  // для обработки продолжения программы
    unsigned int parseExpr();
    unsigned int parseOrExpr();
    unsigned int parseOrPrime(unsigned int left);
    unsigned int parseAndExpr();
    unsigned int parseAndPrime(unsigned int left);
    unsigned int parseShiftExpr();
    unsigned int parseShiftPrime(unsigned int left);
    unsigned int parseUnaryExpr();
    unsigned int parsePrimaryExpr();
    
    void error(const std::string& msg);
    void checkToken();
};

#endif
