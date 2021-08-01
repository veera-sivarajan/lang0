# pragma once

# include <any>
# include <string>
# include "./Token.hpp"
# include "./Expression.hpp"
# include "./Statement.hpp"
# include "./Error.hpp"
# include "./Environment.hpp"

// An interpreter object is also an object of ExprVisitor
// because interpreter is a subclass of ExprVisitor or ExprVisitor
// is the base class of Interpreter

class Interpreter: public ExprVisitor, public StmtVisitor {
private:
    std::shared_ptr<Env> curr_env{new Env};

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
    std::any visitVariableExpr(std::shared_ptr<Variable> expr) override;
    std::any visitAssignExpr(std::shared_ptr<Assign> expr) override;
    std::any visitLogicalExpr(std::shared_ptr<Logical> expr) override;

    std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) override;
    std::any visitPrintStmt(std::shared_ptr<Print> stmt) override;
    std::any visitBlockStmt(std::shared_ptr<Block> stmt) override;
    std::any visitVarStmt(std::shared_ptr<Var> stmt) override;
    std::any visitIfStmt(std::shared_ptr<If> stmt) override;
    std::any visitWhileStmt(std::shared_ptr<While> stmt) override;

    void interpret(std::vector<std::shared_ptr<Stmt>> &statements);
    void execute(std::shared_ptr<Stmt> statement);
    void executeBlock(const std::vector<std::shared_ptr<Stmt>> &statements,
                      std::shared_ptr<Env> new_env);
};
