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

std::any Interpreter::visitBinaryExpr(std::shared_ptr<Binary> expr) {
    std::any left = evaluate(expr->left);
    std::any right = evaluate(expr->right);

    switch (expr->oper.type) {
    case TokenType::MINUS:
        checkNumberOperands(expr->oper, left, right);
        return std::any_cast<int>(left) - std::any_cast<int>(right);

    case TokenType::SLASH:
        checkNumberOperands(expr->oper, left, right);
        return std::any_cast<int>(left) / std::any_cast<int>(right);

    case TokenType::STAR:
        checkNumberOperands(expr->oper, left, right);
        return std::any_cast<int>(left) * std::any_cast<int>(right);
    }
    
}
