#ifndef SCANNER_H
#define SCANNER_H

#include <string>

enum TokenType {
    TOK_INT,      
    TOK_OR,      
    TOK_AND,      
    TOK_SHL,      // '<<'
    TOK_SHR,      // '>>'
    TOK_NOT,      // '~'
    TOK_LPAREN,   // '('
    TOK_RPAREN,   // ')'
    TOK_END,     
    TOK_ERROR     
};

struct Token {
    TokenType type;
    unsigned int value;  // только для TOK_INT 
    std::string lexeme;
};

class Scanner {
public:
    Scanner(const std::string& input);
    Token getNextToken();
    int getPosition() const { return pos; }
    
private:
    std::string input;
    size_t pos;
    char currentChar;
    
    void advance();
    void skipWhitespace();
    Token scanNumber();
    Token scanOperator();
};

#endif