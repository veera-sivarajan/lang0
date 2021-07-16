# include "./Token.hpp"

# include <string>
# include <iostream>
# include <vector>
# include <map>

using std::string;
using std::vector;
using std::map;
using std::stod;
using std::endl;

class Scanner {
private:
    int start = 0;   // first character in the lexeme being scanned
    int current = 0; // character currently under consideration
    int line = 1;    // line # of current
    string source;
    vector<Token> tokens;
    map<string, TokenType> keywords = {
        {"and", AND},
        {"class", CLASS},
        {"else", ELSE},
        {"false", FALSE},
        {"for", FOR},
        {"fun", FUN},
        {"if", IF},
        {"nil", NIL},
        {"or", OR},
        {"print", PRINT},
        {"return", RETURN},
        {"super", SUPER},
        {"this", THIS},
        {"true", TRUE},
        {"var", VAR},
        {"while", WHILE}
    };

    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
    bool isDigit(char c);
    bool match(char expected);

    void scanToken();
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, Object literal);
    char peek();
    char peekNext();
    void makeString();
    void makeNumber();
    void makeIdentifier();
    
public:
    void setSource(string source);
    vector<Token> scanTokens();
    bool isAtEnd();
};
    
