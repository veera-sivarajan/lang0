# include "./Expression.hpp"

using std::any;

Binary::Binary(std::shared_ptr<Expr> left, Token oper,
               std::shared_ptr<Expr> right) :
    left{std::move(left)}, oper{std::move(oper)}, right{std::move(right)} {}

any Binary::accept(ExprVisitor &visitor) {
    return visitor.visitBinaryExpr(shared_from_this());
}

Grouping::Grouping(std::shared_ptr<Expr> expression) :
    expression{std::move(expression)} {}

any Grouping::accept(ExprVisitor &visitor) {
    return visitor.visitGroupingExpr(shared_from_this());
}

Literal::Literal(any value) :
    value{std::move(value)} {}

any Literal::accept(ExprVisitor &visitor) {
    return visitor.visitLiteralExpr(shared_from_this());
}

Unary::Unary(Token oper, std::shared_ptr<Expr> right) :
    oper{std::move(oper)}, right{std::move(right)} {}

any Unary::accept(ExprVisitor &visitor) {
    return visitor.visitUnaryExpr(shared_from_this());
}

Variable::Variable(Token name) :
    name(name) {}

std::any Variable::accept(ExprVisitor &visitor) {
    return visitor.visitVariableExpr(shared_from_this());
}
