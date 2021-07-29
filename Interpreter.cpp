# include "./Interpreter.hpp"

void Interpreter::checkNumberOperand(const Token &oper,
                                     const std::any &operand) {
    if (operand.type() == typeid(int)) return;
    throw RuntimeError{oper, "Operand must be a number."};
}

void Interpreter::checkNumberOperands(const Token &oper, const std::any &left,
                                      const std::any &right) {
    if (left.type() == typeid(int) && right.type() == typeid(int)) return;
    throw RuntimeError{oper, "Operand must be a number."};
}

bool Interpreter::isEqual(const std::any &left, const std::any &right) {
    if (left.type() == typeid(nullptr) && right.type() == typeid(nullptr)) {
        return true;
    }

    if (left.type() == typeid(nullptr)) {
        return false;
    }

    if (left.type() == typeid(int) && right.type() == typeid(int)) {
        return std::any_cast<int>(left) == std::any_cast<int>(right);
    }

    if (left.type() == typeid(std::string) &&
        right.type() == typeid(std::string)) {
        return std::any_cast<string>(left) == std::any_cast<string>(right);
    }

    if (left.type() == typeid(bool) && right.type() == typeid(bool)) {
        return std::any_cast<bool>(left) == std::any_cast<bool>(right);
    }

    return false;
}

std::any Interpreter::visitBinaryExpr(std::shared_ptr<Binary> expr) {
    std::any left = evaluate(expr->left);
    std::any right = evaluate(expr->right);

    switch (expr->oper.type) {

    case TokenType::EQUAL_EQUAL:
        return isEqual(left, right);

    case TokenType::BANG_EQUAL:
        return !isEqual(left, right);

    case TokenType::GREATER:
        checkNumberOperands(expr->oper, left, right);
        return std::any_cast<int>(left) > std::any_cast<int>(right);

    case TokenType::GREATER_EQUAL:
        checkNumberOperands(expr->oper, left, right);
        return std::any_cast<int>(left) >= std::any_cast<int>(right);

    case TokenType::LESS:
        checkNumberOperands(expr->oper, left, right);
        return std::any_cast<int>(left) < std::any_cast<int>(right);

    case TokenType::LESS_EQUAL:
        checkNumberOperands(expr->oper, left, right);
        return std::any_cast<int>(left) <= std::any_cast<int>(right);

    case TokenType::MINUS:
        checkNumberOperands(expr->oper, left, right);
        return std::any_cast<int>(left) - std::any_cast<int>(right);

    case TokenType::PLUS:
        if (left.type() == typeid(int) && right.type() == typeid(int)) {
            return std::any_cast<int>(left) + std::any_cast<int>(right);
        } 
        if (left.type() == typeid(string) && right.type() == typeid(string)) {
            return std::any_cast<string>(left) + std::any_cast<string>(right);
        }
        throw RuntimeError{expr->oper, "Operands not of same type."};
        
    case TokenType::SLASH:
        checkNumberOperands(expr->oper, left, right);
        return std::any_cast<int>(left) / std::any_cast<int>(right);

    case TokenType::STAR:
        checkNumberOperands(expr->oper, left, right);
        return std::any_cast<int>(left) * std::any_cast<int>(right);
    default:
        return {};
    }
}

std::any Interpreter::visitLiteralExpr(std::shared_ptr<Literal> expr) {
    return expr->value;
}

bool Interpreter::isTruthy(const std::any &object) {
    if (object.type() == typeid(nullptr)) return false;
    if (object.type() == typeid(bool)) {
        return std::any_cast<bool>(object);
    }
    return true;
}

std::any Interpreter::visitUnaryExpr(std::shared_ptr<Unary> expr) {
    std::any right = evaluate(expr->right);

    switch (expr->oper.type) {
    case TokenType::BANG:
        return !isTruthy(right);
    case TokenType::MINUS:
        checkNumberOperand(expr->oper, right);
        return -std::any_cast<int>(right);
    default:
        return {};
    }
}

std::string Interpreter::stringify(const std::any &object) {
    if (object.type() == typeid(nullptr)) return "nil";

    if (object.type() == typeid(int)) {
        std::string text = std::to_string(std::any_cast<int>(object));
        return text;
    }

    if (object.type() == typeid(std::string)) {
        return std::any_cast<std::string>(object);
    }

    if (object.type() == typeid(bool)) {
        if (std::any_cast<bool>(object)) {
            string result{"true"};
            return result;
        } else {
            string result{"false"};
            return result;
        }
    }

    return "stringify: cannot recognize type";
}

std::any Interpreter::evaluate(std::shared_ptr<Expr> expr) {
    return expr->accept(*this);
}

std::any Interpreter::visitExpressionStmt(std::shared_ptr<Expression> stmt) {
    std::any value = evaluate(stmt->expression);
    return {};
}

std::any Interpreter::visitPrintStmt(std::shared_ptr<Print> stmt) {
    std::any value = evaluate(stmt->expression);
    std::cout << stringify(value) << "\n";
    return {};
}

// void Interpreter::interpret(std::shared_ptr<Expr> expr) {
//     try {
//         std::any value = evaluate(expr);
//         std::cout << stringify(value) <<  "\n";
//     } catch (RuntimeError &error) {
//         Error::runtimeError(error);
//     }
// }

void Interpreter::execute(std::shared_ptr<Stmt> statement) {
    statement->accept(*this);
}

void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> &statements) {
    try {
        for (std::shared_ptr<Stmt> &statement : statements) {
            execute(statement);
        }
    } catch (RuntimeError &error) {
        Error::runtimeError(error);
    }
}

std::any Interpreter::visitGroupingExpr(std::shared_ptr<Grouping> expr) {
    return evaluate(expr->expression);
}

std::any Interpreter::visitBlockStmt(std::shared_ptr<Block> stmt) {
    return {};
}

std::any Interpreter::visitVarStmt(std::shared_ptr<Var> stmt) {
    std::any value = nullptr;
    if (stmt->init != nullptr) {
        value = evaluate(stmt->init);
    }
    environment->define(stmt->name.text, std::move(value));
    return {};
}

std::any Interpreter::visitVariableExpr(std::shared_ptr<Variable> expr) {
    return environment->get(expr->name);
}

std::any Interpreter::visitAssignExpr(std::shared_ptr<Assign> expr) {
    std::any value = evaluate(expr->value);
    environment->assign(expr->name, value);
    return value;
}
