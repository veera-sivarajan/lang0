# pragma once

# include <any>
# include <string>

class Interpreter: public ExprVisitor {
private:
    void checkNumberOperand(const Token &oper, const std::any &operand);
    void checkNumberOperands(const Token &oper, const std::any &left,
                             const std::any &right);

public:
    std::any visitBinaryExpr(std::shared_ptr<Binary> expr);
