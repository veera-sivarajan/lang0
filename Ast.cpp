# include "./Ast.hpp"

std::string Ast::print(std::shared_ptr<Expr> expr) {
    try {
        return std::any_cast<std::string>(expr->accept(*this));
    }
    catch (const std::bad_any_cast &e) {
        std::cout << "Ast::print -> " << e.what() << "\n";
        return "something";
    }
}

any Ast::visitBinaryExpr(std::shared_ptr<Binary> expr) {
    try {
        return parenthesize(expr->oper.text, expr->left, expr->right);
    }
    catch (const std::bad_any_cast &e) {
        std::cout << "Ast::visitBinaryExpr -> " << e.what() << "\n";
    }
}

any Ast::visitGroupingExpr(std::shared_ptr<Grouping> expr) {
    try {
        return parenthesize("group", expr->expression);
    }
    catch (const std::bad_any_cast &e) {
        std::cout << "Ast::visitGroupingExpr -> " << e.what() << "\n";
    }
}

any Ast::visitLiteralExpr(std::shared_ptr<Literal> expr) {
    auto &type = expr->value.type();

    if (type == typeid(nullptr)) return "nil";
    else if (type == typeid(std::string)) {
        try {
            return std::any_cast<std::string>(expr->value);
        } catch (const std::bad_any_cast &e) {
            std::cout << "Ast::visitLiteralExpr -> " << e.what() << "\n";
        }
    }
    else if (type == typeid(int)) {
        try {
            return std::to_string(std::any_cast<int>(expr->value));
        } catch (const std::bad_any_cast &e) {
            std::cout << "Ast::visitLiteralExpr::type == int -> " << e.what()
                      << "\n";
        }
    }
    else if (type == typeid(bool)) {
        try {
            return std::any_cast<bool>(expr->value) ? "true" : "false";
        } catch (const std::bad_any_cast &e) {
            std::cout << "Ast::visitLiteralExpr::type == bool-> " << e.what()
                      << "\n";
        }
    }
    return "Literal type not recognized";
}
        
any Ast::visitUnaryExpr(std::shared_ptr<Unary> expr) {
    return parenthesize(expr->oper.text, expr->right);
}

