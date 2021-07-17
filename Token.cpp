# include "./Token.hpp"
# include "./magic_enum.hpp"

# include <string>
# include <iostream>

using std::string;

Token::Token(TokenType type, int line, int length, string lexeme) :
    type(type), line(line), length(length), lexeme(lexeme) {}

void Token::print() {
    std::cout << magic_enum::enum_name(this->type) << " "
              << this->lexeme << "\n";
}

