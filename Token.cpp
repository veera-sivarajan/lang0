# include "./Token.hpp"
# include "./magic_enum.hpp"

# include <string>
# include <iostream>

using std::string;

void Token::print() {
    std::cout << magic_enum::enum_name(this->type) << " "
              << this->lexeme << "\n";
}

