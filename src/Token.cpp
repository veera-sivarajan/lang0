# include "../include/Token.hpp"

# include <string>
# include <iostream>

using std::string;
using std::any;

Token::Token(TokenType type, int line, string text, any literal) :
    type(type), line(line), text(text), literal(literal) {}

bool Token::operator<(const Token& obj) const {
    return obj.text < this->text;
}

void Token::print() {
    std::cout << magic_enum::enum_name(this->type) << " "
              << text << "\n";
}

