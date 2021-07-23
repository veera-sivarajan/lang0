# include <vector>
# include "./Scanner.hpp"
# include "./Expression.hpp"

using std::vector;

class Parser {
private:
    const vector<Token> tokens;  // consume tokens from scanner
    int current = 0; // points to next token 

    Token peek();
    Token previous();
    Token advance();
    bool isAtEnd();
    bool check(TokenType type);
    bool match(vector<TokenType> types);

    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> equality();


public:
    Parser(const vector<Token> tokens);
};


