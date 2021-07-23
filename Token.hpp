# include <iostream>
# include <string>

using std::string;

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
private:
    int line; 
    int length ;
    string lexeme;

public:
    Token(TokenType type, int line, int length, string lexeme);
    TokenType type;
    void print();
};
    
    
