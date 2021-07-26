# pragma once

# include <any>
# include <string>
# include "./RuntimeError.hpp"
# include "./Token.hpp"
# include "./Expression.hpp"

class Interpreter: public ExprVisitor {
private:
    void checkNumberOperand(const Token &oper, const std::any &operand);
    void checkNumberOperands(const Token &oper, const std::any &left,
                             const std::any &right);

    bool isEqual(const std::any &left, const std::any &right);

public:
    std::any visitBinaryExpr(std::shared_ptr<Binary> expr);
