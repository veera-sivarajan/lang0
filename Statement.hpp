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

struct StmtVisitor {
    virtual std::any visitBlockStmt(std::shared_ptr<Block> stmt) = 0;
    virtual std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) = 0;
    virtual std::any visitPrintStmt(std::shared_ptr<Print> stmt) = 0;
    virtual std::any visitVarStmt(std::shared_ptr<Var> stmt) = 0;
    virtual std::any visitIfStmt(std::shared_ptr<If> stmt) = 0;
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
