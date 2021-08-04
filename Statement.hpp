# pragma once 

# include <any>
# include <vector>
# include <memory>

# include "./Token.hpp"
# include "./Expression.hpp"

struct Block;
struct Expression;
struct Print;
struct Var;
struct If;
struct While;
struct Function;
struct Return;

struct StmtVisitor {
    virtual std::any visitBlockStmt(std::shared_ptr<Block> stmt) = 0;
    virtual std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) = 0;
    virtual std::any visitPrintStmt(std::shared_ptr<Print> stmt) = 0;
    virtual std::any visitVarStmt(std::shared_ptr<Var> stmt) = 0;
    virtual std::any visitIfStmt(std::shared_ptr<If> stmt) = 0;
    virtual std::any visitWhileStmt(std::shared_ptr<While> stmt) = 0;
    virtual std::any visitFunctionStmt(std::shared_ptr<Function> stmt) = 0;
    virtual std::any visitReturnStmt(std::shared_ptr<Return> stmt) = 0;
    virtual ~StmtVisitor() = default;
};

struct Stmt {
    virtual std::any accept(StmtVisitor &visitor) = 0;
};

struct Block: Stmt, public std::enable_shared_from_this<Block> {
    const std::vector<std::shared_ptr<Stmt>> statements;

    Block(std::vector<std::shared_ptr<Stmt>> statements);
    std::any accept(StmtVisitor &visitor) override;
};

struct Expression: Stmt, public std::enable_shared_from_this<Expression> {
    const std::shared_ptr<Expr> expression;

    Expression(std::shared_ptr<Expr> expression);
    std::any accept(StmtVisitor &visitor) override;
};

struct Print: Stmt, public std::enable_shared_from_this<Print> {
    const std::shared_ptr<Expr> expression;

    Print(std::shared_ptr<Expr> expression);
    std::any accept(StmtVisitor &visitor) override;
};

struct Var: Stmt, public std::enable_shared_from_this<Var> {
    const Token name;
    std::shared_ptr<Expr> init;

    Var(Token name, std::shared_ptr<Expr> init);
    std::any accept(StmtVisitor &visitor) override;
};

struct If: Stmt, public std::enable_shared_from_this<If> {
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> thenBranch;
    std::shared_ptr<Stmt> elseBranch;

    If(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> thenBranch,
       std::shared_ptr<Stmt> elseBranch);
    std::any accept(StmtVisitor &visitor) override;
};

struct While: Stmt, public std::enable_shared_from_this<While> {
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> body;

    While(std::shared_ptr<Expr> condition, std::shared_ptr<Stmt> body);
    std::any accept(StmtVisitor &visitor) override;
};

struct Function: Stmt, public std::enable_shared_from_this<Function> {
    Token name;
    std::vector<Token> params;
    std::vector<std::shared_ptr<Stmt>> body;

    Function(Token name, std::vector<Token> params,
             std::vector<std::shared_ptr<Stmt>> body);
    std::any accept(StmtVisitor &visitor) override;
};

struct Return: Stmt, public std::enable_shared_from_this<Return> {
    Token keyword;
    std::shared_ptr<Expr> value;

    Return(Token keyword, std::shared_ptr<Expr> value);
    std::any accept(StmtVisitor &visitor) override;
};
