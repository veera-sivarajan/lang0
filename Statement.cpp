# include "./Statement.hpp"

Block::Block(std::vector<std::shared_ptr<Stmt>> statements) :
    statements{std::move(statements)} {}

std::any Block::accept(StmtVisitor &visitor) {
    return visitor.visitBlockStmt(shared_from_this());
}

Expression::Expression(std::shared_ptr<Expr> expression) :
    expression(expression) {}

std::any Expression::accept(StmtVisitor &visitor) {
    return visitor.visitExpressionStmt(shared_from_this());
}

Print::Print(std::shared_ptr<Expr> expression) :
    expression(expression) {}

std::any Print::accept(StmtVisitor &visitor) {
    return visitor.visitPrintStmt(shared_from_this());
}

Var::Var(Token name, std::shared_ptr<Expr> init) :
    name(name), init(init) {}

std::any Var::accept(StmtVisitor &visitor) {
    return visitor.visitVarStmt(shared_from_this());
}
