# include <vector>
# include <stdexcept>
# include "./Scanner.hpp"
# include "./Expression.hpp"
# include "./Dlox.hpp"

using std::vector;

class Parser {
private:
    struct ParseError: public std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    const vector<Token> tokens;  // consume tokens from scanner
    int current = 0; // points to next token 

    ParseError error(Token token, string message);

    void synchronize();
    
    Token peek();
    Token previous();
    Token advance();
    Token consume(TokenType type, string message);

    bool isAtEnd();
    bool check(TokenType type);
    bool match(vector<TokenType> types);

    std::shared_ptr<Expr> parse();
    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> primary();


public:
    Parser(const vector<Token> tokens);
};


