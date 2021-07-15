# include "./Token.hpp"

# include <string>
# include <iostream>

using std::string;


Token::Token (TokenType type, string lexeme, int line) {
    type = type;
    lexeme = lexeme;
    line = line;
}

string Token::toString () {
    return type + " " + lexeme;
}

