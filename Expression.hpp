# include <utility>
# include <vector>
# include <memory> 
# include <any>

struct Binary;
struct Grouping;
struct Literal;
struct Unary;

struct ExprVisitor {
    //pure virutal functions
    virtual std::any visitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
    virtual std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
    virtual std::any visitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
    virtual std::any visitUnaryExpr(std::shared_ptr<Unary> expr) = 0;
    virtual ~ExprVisitor() = default;
};

struct Expr {
    virtual std::any accept(ExprVisitor &visitor) = 0;
};

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
