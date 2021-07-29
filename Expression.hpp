# pragma once
# include <utility>
# include <vector>
# include <memory> 
# include <any>

# include "./Token.hpp"

using std::any;

struct Binary;
struct Grouping;
struct Literal;
struct Unary;
struct Variable;
struct Assign;

struct ExprVisitor {
    //pure virutal functions
    virtual any visitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
    virtual any visitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
    virtual any visitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
    virtual any visitUnaryExpr(std::shared_ptr<Unary> expr) = 0;
    virtual any visitVariableExpr(std::shared_ptr<Variable> expr) = 0;
    virtual any visitAssignExpr(std::shared_ptr<Assign> expr) = 0;
    virtual ~ExprVisitor() = default;
};

struct Expr {
    virtual any accept(ExprVisitor &visitor) = 0;
};

struct Binary: Expr, public std::enable_shared_from_this<Binary> {
    std::shared_ptr<Expr> left;
    Token oper;
    std::shared_ptr<Expr> right;

    Binary(std::shared_ptr<Expr> left, Token oper, std::shared_ptr<Expr> right);
    any accept(ExprVisitor &visitor) override;
};

struct Grouping: Expr, public std::enable_shared_from_this<Grouping> {
    std::shared_ptr<Expr> expression;

    Grouping(std::shared_ptr<Expr> expression);
    any accept(ExprVisitor &visitor) override;
};

struct Literal: Expr, public std::enable_shared_from_this<Literal> {
    any value;

    Literal(any value);
    any accept(ExprVisitor &visitor) override;
};

struct Unary: Expr, public std::enable_shared_from_this<Unary> {
    Token oper;
    std::shared_ptr<Expr> right;

    Unary(Token oper, std::shared_ptr<Expr> right);
    any accept(ExprVisitor &visitor) override;
};

struct Variable: Expr, public std::enable_shared_from_this<Variable> {
    Token name;

    Variable(Token name);
    std::any accept(ExprVisitor &visitor) override;
};

struct Assign: Expr, public std::enable_shared_from_this<Assign> {
    const Token name;
    const std::shared_ptr<Expr> value;

    Assign(Token name, std::shared_ptr<Expr> value);
    std::any accept(ExprVisitor &visitor) override;
};
        
