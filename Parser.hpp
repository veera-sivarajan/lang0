# include <iostream>
# include <vector>

# include "./Token.hpp"

using std::vector;

class Parser {
private:
    int current = 0;
    vector<Token> tokens;
    Expr expression();

public:
    Parser();
};
