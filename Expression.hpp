# pragma once
# include <utility>
# include <vector>
# include <memory> 
# include <any>

# include "./Token.hpp"
# include "./Visitor.hpp"

struct Binary: Expr, public std::enable_shared_from_this<Binary> {
    std::shared_ptr<Expr> left;
    Token oper;
    std::shared_ptr<Expr> right;

    Binary(std::shared_ptr<Expr> left, Token oper, std::shared_ptr<Expr> right);
    std::any accept(ExprVisitor &visitor) override;
};

struct Grouping: Expr, public std::enable_shared_from_this<Grouping> {
    std::shared_ptr<Expr> expression;

    Grouping(std::shared_ptr<Expr> expression);
    std::any accept(ExprVisitor &visitor) override;
};

struct Literal: Expr, public std::enable_shared_from_this<Literal> {
    std::any value;

    Literal(std::any value);
    std::any accept(ExprVisitor &visitor) override;
};

struct Unary: Expr, public std::enable_shared_from_this<Unary> {
    Token oper;
    std::shared_ptr<Expr> right;

    Unary(Token oper, std::shared_ptr<Expr> right);
    std::any accept(ExprVisitor &visitor) override;
};

struct Variable: Expr, public std::enable_shared_from_this<Variable> {
    Token name;

    Variable(Token name);
    std::any accept(ExprVisitor &visitor) override;
};

struct Assign: Expr, public std::enable_shared_from_this<Assign> {
    Token name;
    std::shared_ptr<Expr> value;

    Assign(Token name, std::shared_ptr<Expr> value);
    std::any accept(ExprVisitor &visitor) override;
};
        
struct Logical: Expr, public std::enable_shared_from_this<Logical> {
    std::shared_ptr<Expr> left;
    Token oper;
    std::shared_ptr<Expr> right;

    Logical(std::shared_ptr<Expr> left, Token oper,
            std::shared_ptr<Expr> right);
    std::any accept(ExprVisitor &visitor) override;
};

struct Call: Expr, public std::enable_shared_from_this<Call> {
    std::shared_ptr<Expr> callee;
    Token paren;
    std::vector<std::shared_ptr<Expr>> arguments;

    Call(std::shared_ptr<Expr> callee, Token paren,
         std::vector<std::shared_ptr<Expr>> arguments);
    std::any accept(ExprVisitor &visitor) override;
};

struct Lambda: Expr, public std::enable_shared_from_this<Lambda> {
    std::vector<Token> params;
    std::vector<std::shared_ptr<Stmt>> body;

    Lambda(std::vector<Token> params, std::vector<std::shared_ptr<Stmt>> body);
    std::any accept(ExprVisitor &visitor) override;
};

struct List: Expr, public std::enable_shared_from_this<List> {
    std::vector<std::shared_ptr<Expr>> values;

    List(std::vector<std::shared_ptr<Expr>> values);
    std::any accept(ExprVisitor &visitor) override;
};

struct Subscript: Expr, public std::enable_shared_from_this<Subscript> {
    std::shared_ptr<Expr> listName;
    std::shared_ptr<Expr> index;

    Subscript(std::shared_ptr<Expr> listName, std::shared_ptr<Expr> index);
    std::any accept(ExprVisitor &visitor) override;
};


