#include "scanner.h"
#include <cctype>
#include <stdexcept>
#include <climits>

Scanner::Scanner(const std::string& input) 
    : input(input), pos(0) {
    if (!input.empty()) {
        currentChar = input[0];
    } else {
        currentChar = '\0';
    }
}

void Scanner::advance() {
    if (pos < input.length()) {
        pos++;
        if (pos < input.length()) {
            currentChar = input[pos];
        } else {
            currentChar = '\0';
        }
    }
}

void Scanner::skipWhitespace() {
    while (currentChar != '\0' && isspace(currentChar)) {
        advance();
    }
}

Token Scanner::scanNumber() {
    std::string numStr;
    
    while (currentChar != '\0' && isdigit(currentChar)) {
        numStr += currentChar;
        advance();
    }
    
    if (currentChar != '\0' && isalpha(currentChar)) {
        return {TOK_ERROR, 0, std::string("Lexical error: Unexpected character: ") + currentChar};
    }

    unsigned long long val = 0;
    for (char c : numStr) {
        val = val * 10 + (c - '0');
        if (val > UINT_MAX) {
            return {TOK_ERROR, 0, "Lexical error: Integer overflow in literal"};
        }
    }
    
    return {TOK_INT, (unsigned int)val, numStr};
}

Token Scanner::scanOperator() {
    char op = currentChar;
    std::string lexeme(1, op);
    advance();
    
    if ((op == '<' || op == '>') && currentChar == op) {
        lexeme += currentChar;
        advance();
        return {op == '<' ? TOK_SHL : TOK_SHR, 0, lexeme};
    }
    
    switch (op) {
        case '|': return {TOK_OR, 0, lexeme};
        case '&': return {TOK_AND, 0, lexeme};
        case '~': return {TOK_NOT, 0, lexeme};
        case '(': return {TOK_LPAREN, 0, lexeme};
        case ')': return {TOK_RPAREN, 0, lexeme};
        case ';': return {TOK_SEMICOLON, 0, lexeme};  // <-- ДОБАВЛЯЕМ
        default:  return {TOK_ERROR, 0, "Unknown operator"};
    }
}

Token Scanner::getNextToken() {
    skipWhitespace();
    
    if (currentChar == '\0') {
        return {TOK_END, 0, ""};
    }
    
    if (isdigit(currentChar)) {
        return scanNumber();
    }
    
    if (currentChar == '|' || currentChar == '&' || currentChar == '~' || 
        currentChar == '(' || currentChar == ')' || currentChar == ';' ||
        currentChar == '<' || currentChar == '>') {
        return scanOperator();
    }
    
    char errChar = currentChar;
    advance();
    return {TOK_ERROR, 0, std::string("Lexical error: Unexpected character: ") + errChar};
}
