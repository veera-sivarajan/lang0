# include "./Ast.hpp"

string Ast::print(std::shared_ptr<Expr> expr) {
    return std::any_cast<string>(expr->accept(*this));
}

any Ast::visitBinaryExpr(std::shared_ptr<Binary> expr) {
    return parenthesize(expr->oper.text, expr->left, expr->right);
}

any Ast::visitGroupingExpr(std::shared_ptr<Grouping> expr) {
    return parenthesize("group", expr->expression);
}

any Ast::visitLiteralExpr(std::shared_ptr<Literal> expr) {
    auto &type = expr->value.type();

    if (type == typeid(nullptr)) return "nil";
    else if (type == typeid(string)) return std::any_cast<string>(expr->value);
    else if (type == typeid(double)) return std::any_cast<double>(expr->value);
    else if (type == typeid(bool))
        return std::any_cast<bool>(expr->value) ? "true" : "false";

    return "Literalr type not recognized";
}
        
any Ast::visitUnaryExpr(std::shared_ptr<Unary> expr) {
    return parenthesize(expr->oper.text, expr->right);
}

template <class... E>
string Ast::parenthesize(string name, E... expr) {
    std::ostringstream buffer;
    buffer << "(" << name;
    ((buffer << " " << print(expr)), ...);
    buffer << ")";
    return buffer.str();
}

