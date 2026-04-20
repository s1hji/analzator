#include "parser.h"
#include <iostream>

Parser::Parser(Scanner& scanner) : scanner(scanner) {
    currentToken = scanner.getNextToken();
}

void Parser::error(const std::string& msg) {
    if (msg.find("Lexical error") == 0) {
        throw std::runtime_error(msg);
    } else {
        throw std::runtime_error("Syntax error at position " + std::to_string(scanner.getPosition()) + ": " + msg);
    }
}

void Parser::consume(TokenType expected) {
    if (currentToken.type == expected) {
        currentToken = scanner.getNextToken();
    } else {
        error("Expected token " + std::to_string(expected) + ", got " + std::to_string(currentToken.type));
    }
}

// Парсинг одного выражения (для обратной совместимости)
unsigned int Parser::parse() {
    unsigned int result = parseExpr();
    if (currentToken.type != TOK_END) {
        error("Unexpected tokens after expression");
    }
    return result;
}

// Парсинг программы: <program> ::= <expr> <program'>
std::vector<unsigned int> Parser::parseProgram() {
    std::vector<unsigned int> results;
    
    // Парсим первое выражение
    if (currentToken.type == TOK_END) {
        return results;  // пустая программа
    }
    
    unsigned int firstResult = parseExpr();
    results.push_back(firstResult);
    
    // Парсим продолжение
    while (currentToken.type == TOK_SEMICOLON) {
        consume(TOK_SEMICOLON);
        if (currentToken.type == TOK_END) {
            error("Expected expression after ';'");
        }
        unsigned int nextResult = parseExpr();
        results.push_back(nextResult);
    }
    
    if (currentToken.type != TOK_END) {
        error("Unexpected tokens after expression");
    }
    
    return results;
}

void Parser::checkToken() {
    if (currentToken.type == TOK_ERROR) {
        error(currentToken.lexeme);
    }
}

unsigned int Parser::parseExpr() {
    return parseOrExpr();
}

unsigned int Parser::parseOrExpr() {
    checkToken(); 
    unsigned int left = parseAndExpr();
    return parseOrPrime(left);
}

unsigned int Parser::parseOrPrime(unsigned int left) {
    checkToken(); 
    if (currentToken.type == TOK_OR) {
        consume(TOK_OR);
        unsigned int right = parseAndExpr();
        return parseOrPrime(left | right);
    }
    return left;
}

unsigned int Parser::parseAndExpr() {
    checkToken(); 
    unsigned int left = parseShiftExpr();
    return parseAndPrime(left);
}

unsigned int Parser::parseAndPrime(unsigned int left) {
    checkToken(); 
    if (currentToken.type == TOK_AND) {
        consume(TOK_AND);
        unsigned int right = parseShiftExpr();
        return parseAndPrime(left & right);
    }
    return left;
}

unsigned int Parser::parseShiftExpr() {
    checkToken(); 
    unsigned int left = parseUnaryExpr();
    return parseShiftPrime(left);
}

unsigned int Parser::parseShiftPrime(unsigned int left) {
    checkToken(); 
    if (currentToken.type == TOK_SHL) {
        consume(TOK_SHL);
        unsigned int right = parseUnaryExpr();
        
        if (right >= 32) {
            error("Shift amount " + std::to_string(right) + " must be < 32");
        }
        
        return parseShiftPrime(left << right);
    } else if (currentToken.type == TOK_SHR) {
        consume(TOK_SHR);
        unsigned int right = parseUnaryExpr();
        
        if (right >= 32) {
            error("Shift amount " + std::to_string(right) + " must be < 32");
        }
        
        return parseShiftPrime(left >> right);
    }
    return left;
}

unsigned int Parser::parseUnaryExpr() {
    checkToken(); 
    if (currentToken.type == TOK_NOT) {
        consume(TOK_NOT);
        unsigned int val = parseUnaryExpr();
        return ~val;
    } else {
        return parsePrimaryExpr();
    }
}

unsigned int Parser::parsePrimaryExpr() {
    checkToken(); 
    if (currentToken.type == TOK_ERROR) {
        error(currentToken.lexeme);
    }

    if (currentToken.type == TOK_LPAREN) {
        consume(TOK_LPAREN);
        unsigned int val = parseExpr();
        if (currentToken.type != TOK_RPAREN) {
            error("Expected ')'");
        }
        consume(TOK_RPAREN);
        return val;
    } else if (currentToken.type == TOK_INT) {
        unsigned int val = currentToken.value;
        consume(TOK_INT);
        return val;
    } else {
        error("Expected '(' or integer");
        return 0;
    }
}
