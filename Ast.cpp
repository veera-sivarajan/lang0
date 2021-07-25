# include "./Ast.hpp"

std::string Ast::print(std::shared_ptr<Expr> expr) {
    try {
        return std::any_cast<std::string>(expr->accept(*this));
    } catch (const std::bad_any_cast &e) {
        std::cout << "Ast::print -> " << e.what() << "\n";
        return "something";
    }
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
    else if (type == typeid(std::string)) {
        try {
            return std::any_cast<std::string>(expr->value);
        } catch (const std::bad_any_cast &e) {
            std::cout << "Ast::visitLiteralExpr -> " << e.what() << "\n";
        }
    } else if (type == typeid(int)) {
        return std::to_string(std::any_cast<int>(expr->value));
    } else if (type == typeid(bool)) {
        if (std::any_cast<bool>(expr->value)) {
            std::string result{"true"};
            return result;
        } else {
            std::string result{"false"};
            return result;
        }
    }
    return "Literal type not recognized";
}
        
any Ast::visitUnaryExpr(std::shared_ptr<Unary> expr) {
    return parenthesize(expr->oper.text, expr->right);
}

template<class... E>
string Ast::parenthesize(std::string_view name, E... expr) {
    assert((... && std::is_same_v<E, std::shared_ptr<Expr>>));
    std::ostringstream buffer;
    buffer << "(" << name;
    ((buffer << " " << print(expr)), ...);
    buffer << ")";
    return buffer.str();
}
