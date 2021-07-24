# include "./Expression.hpp"

using std::any;

Binary::Binary(std::shared_ptr<Expr> left, Token oper,
               std::shared_ptr<Expr> right) :
    left(left), oper(oper), right(right) {}

any Binary::accept(ExprVisitor &visitor) {
    return visitor.visitBinaryExpr(shared_from_this());
}

Grouping::Grouping(std::shared_ptr<Expr> expression) :
    expression(expression) {}

any Grouping::accept(ExprVisitor &visitor) {
    return visitor.visitGroupingExpr(shared_from_this());
}

Literal::Literal(any value) :
    value(value) {}

any Literal::accept(ExprVisitor &visitor) {
    return visitor.visitLiteralExpr(shared_from_this());
}

Unary::Unary(Token oper, std::shared_ptr<Expr> right) :
    oper(oper), right(right) {}

any Unary::accept(ExprVisitor &visitor) {
    return visitor.visitUnaryExpr(shared_from_this());
}
