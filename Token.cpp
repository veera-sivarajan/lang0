# include "./Token.hpp"
# include "./magic_enum.hpp"

# include <string>
# include <iostream>

using std::string;

Token::Token(TokenType type, int line, int length, string lexeme) :
    line(line), length(length), lexeme(lexeme), type(type) {}

void Token::print() {
    std::cout << magic_enum::enum_name(this->type) << " "
              << this->lexeme << "\n";
}

