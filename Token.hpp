# pragma once
# include <iostream>
# include <string>
# include <any>

# include "./Libs/magic_enum.hpp"

using std::string;
using std::any;


enum class TokenType {
    // Single Character Tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, COMMA, DOT, MINUS,
    PLUS, SEMICOLON, SLASH, STAR,
    // One or two character tokens
    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL, GREATER, GREATER_EQUAL, LESS,
    LESS_EQUAL,
    // Literals
    IDENTIFIER, STRING, NUMBER,
    // Keywords
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR, PRINT, RETURN, SUPER,
    THIS, TRUE, VAR, WHILE,

    EOF_TOKEN                // Using this instead of EOF becuase EOF is keyword
};

class Token {
public:
    TokenType type;
    int line; 
    string text;
    any literal;

    Token(TokenType type, int line, string text, any literal);
    void print();
};
    
    
