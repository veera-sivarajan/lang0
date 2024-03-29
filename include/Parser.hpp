# include <vector>
# include <stdexcept>
# include "./Scanner.hpp"
# include "./Expression.hpp"
# include "./Statement.hpp"
# include "./Error.hpp"
# include "./Ast.hpp"

using std::vector;

class Parser {
private:
    struct ParseError: public std::runtime_error {
        using std::runtime_error::runtime_error;
    };

    const vector<Token> tokens;  // consume tokens from scanner
    int current = 0; // points to next token 
    void peek_print();

    ParseError error(Token token, string message);

    void synchronize();
    
    Token peek();
    Token previous();
    Token advance();
    Token consume(TokenType type, string message);

    bool isAtEnd();
    bool check(TokenType type);
    template<class... T>
    bool match(T... types);

    std::shared_ptr<Expr> expression();
    std::shared_ptr<Expr> equality();
    std::shared_ptr<Expr> comparison();
    std::shared_ptr<Expr> term();
    std::shared_ptr<Expr> factor();
    std::shared_ptr<Expr> unary();
    std::shared_ptr<Expr> primary();
    std::shared_ptr<Expr> assignment();
    std::shared_ptr<Expr> logicalOr();
    std::shared_ptr<Expr> logicalAnd();
    std::shared_ptr<Expr> call();
    std::shared_ptr<Expr> finishCall(std::shared_ptr<Expr> callee);
    std::shared_ptr<Expr> lambda();
    std::shared_ptr<Expr> list();
    std::shared_ptr<Expr> subscript();
    std::shared_ptr<Expr> finishSubscript(std::shared_ptr<Expr> name);

    std::shared_ptr<Statement::Stmt> statement();
    std::shared_ptr<Statement::Stmt> printStatement();
    std::shared_ptr<Statement::Stmt> expressionStatement();
    std::shared_ptr<Statement::Stmt> declaration();
    std::shared_ptr<Statement::Stmt> varDeclaration();
    std::shared_ptr<Statement::Stmt> ifStatement();
    std::shared_ptr<Statement::Stmt> whileStatement();
    std::shared_ptr<Statement::Stmt> forStatement();
    std::shared_ptr<Statement::Stmt> returnStatement();

    std::vector<std::shared_ptr<Statement::Stmt>> block();

    std::shared_ptr<Statement::Function> function(std::string kind);

public:
    Parser(const vector<Token> &tokens);
    // std::shared_ptr<Expr> parse();
    std::vector<std::shared_ptr<Statement::Stmt>> parse();
};


