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
        else if (left.type() == typeid(string) &&
                 right.type() == typeid(string)) {
            return std::any_cast<string>(left) + std::any_cast<string>(right);
        }
        
    case TokenType::SLASH:
        checkNumberOperands(expr->oper, left, right);
        return std::any_cast<int>(left) / std::any_cast<int>(right);

    case TokenType::STAR:
        checkNumberOperands(expr->oper, left, right);
        return std::any_cast<int>(left) * std::any_cast<int>(right);

    }
    
}
