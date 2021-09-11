# pragma once
# include "./Token.hpp"

# include <string>
# include <iostream>
# include <vector>
# include <map>

class Scanner {
private:
    int start = 0;   // first character in the lexeme being scanned
    int current = 0; // character currently under consideration
    int line = 1;    // line # of current
    std::string source;
    std::vector<Token> tokens;
    std::map<std::string, TokenType> keywords = {
        {"and",    TokenType::AND},
        {"class",  TokenType::CLASS},
        {"else",   TokenType::ELSE},
        {"false",  TokenType::FALSE},
        {"for",    TokenType::FOR},
        {"fun",    TokenType::FUN},
        {"if",     TokenType::IF},
        {"nil",    TokenType::NIL},
        {"or",     TokenType::OR},
        {"print",  TokenType::PRINT},
        {"return", TokenType::RETURN},
        {"super",  TokenType::SUPER},
        {"this",   TokenType::THIS},
        {"true",   TokenType::TRUE},
        {"var",    TokenType::VAR},
        {"while",  TokenType::WHILE},
        {"lambda", TokenType::LAMBDA}
    };

    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
    bool isDigit(char c);
    bool match(char expected);

    void scanToken();
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, std::any literal);
    char peek();
    char peekNext();
    void makeString();
    void makeNumber();
    void makeIdentifier();
    
public:
    void setSource(std::string source);
    void printToken(Token token);
    std::vector<Token> scanTokens();
    bool isAtEnd();
};
    
