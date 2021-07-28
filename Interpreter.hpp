# pragma once

# include <any>
# include <string>
# include "./Token.hpp"
# include "./Expression.hpp"
# include "./Error.hpp"

// An interpreter object is also an object of ExprVisitor
// because interpreter is a subclass of ExprVisitor or ExprVisitor
// is the base class of Interpreter

class Interpreter: public ExprVisitor {
private:
    void checkNumberOperand(const Token &oper, const std::any &operand);
    void checkNumberOperands(const Token &oper, const std::any &left,
                             const std::any &right);

    bool isEqual(const std::any &left, const std::any &right);
    bool isTruthy(const std::any &object);

    std::string stringify(const std::any &object);

    std::any evaluate(std::shared_ptr<Expr> expr);

public:
    std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override;
    std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override;
    std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override;
    std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override;

    void interpret(std::shared_ptr<Expr> expr);
};
