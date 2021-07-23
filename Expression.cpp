# include <vector>
# include <memory> 
# include "./Token.h"

using std::vector;
using std::shared_ptr<T>;
using std::enable_shared_from_this<T>;

struct ExprVisitor {
    virtual visitBinaryExpr(shared_ptr<Binary> expr) = 0; //pure virutal function
    virtual visitGroupingExpr(shared_ptr<Grouping> expr) = 0;
    virtual visitLiteralExpr(shared_ptr<Literal> expr) = 0;
    virtual visitUnaryExpr(shared_ptr<Unary> expr) = 0;
    virtual ~ExprVisitor() = default;
};

struct Expr {
    virtual accept(ExprVisitor &visitor) = 0;
};

struct Binary: Expr, public enable_shared_from_this<Binary> {
    Binary(shared_ptr<Expr> left, Token oper, shared_ptr<Expr> right) :
        left(left), oper(oper), right(right) {}

    accept(ExprVisitor &visitor) override {
        return visitor.visitBinaryExpr(shared_from_this());
    }
}

    
