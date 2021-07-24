# include "./Ast.hpp"

std::string Ast::print(std::shared_ptr<Expr> expr) {
    try {
        any temp = expr->accept(*this);
        auto &type = temp.type();
        // std::cout << "Type: " << type << "\n";
        if (type == typeid(string)) {
            // return "string";
            return std::any_cast<std::string>(expr->accept(*this));
        } else {
            return "something";
        }
    }
    catch (const std::bad_any_cast &e) {
        std::cout << "Ast::print -> " << e.what() << "\n";
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
    else if (type == typeid(double)) {
        try {
            return std::to_string(std::any_cast<double>(expr->value));
        } catch (const std::bad_any_cast &e) {
            std::cout << "Ast::visitLiteralExpr::type == double -> " << e.what()
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

// template <class... E>
// std::string Ast::parenthesize(std::string_view name, E... expr) {
//     assert((... && std::is_same_v<E, std::shared_ptr<Expr>>));
//     std::ostringstream buffer;
//     buffer << "(" << name;
//     ((buffer << " " << print(expr)), ...);
//     buffer << ")";
//     return buffer.str();
// }

