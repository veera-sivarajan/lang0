# pragma once

# include <vector>
# include <map>
# include "./Interpreter.hpp"

class Resolver: public ExprVisitor, public StmtVisitor {
private:
    Interpreter& interpreter;
    std::vector<std::map<std::string, bool>> scopes;
    enum class FunctionType {
        NONE,
        FUNCTION,
    };

    FunctionType currentFunction = FunctionType::NONE;
    
    void resolve(std::shared_ptr<Stmt> statement);
    void resolve(std::shared_ptr<Expr> expression);
    void beginScope();
    void endScope();
    void declare(Token& name);
    void define(Token& name);
    void resolveLocal(std::shared_ptr<Expr> expr, Token& name);
    void resolveFunction(std::shared_ptr<Function> function, FunctionType type);

public:
    Resolver(Interpreter& interpreter);
    void resolve(std::vector<std::shared_ptr<Stmt>>& statements);
    
    std::any visitBlockStmt(std::shared_ptr<Block> stmt) override;
    std::any visitVarStmt(std::shared_ptr<Var> stmt) override;
    std::any visitFunctionStmt(std::shared_ptr<Function> stmt) override;
    std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) override;
    std::any visitIfStmt(std::shared_ptr<If> stmt) override;
    std::any visitPrintStmt(std::shared_ptr<Print> stmt) override;
    std::any visitReturnStmt(std::shared_ptr<Return> stmt) override;
    std::any visitWhileStmt(std::shared_ptr<While> stmt) override;

    std::any visitVariableExpr(std::shared_ptr<Variable> expr) override;
    std::any visitAssignExpr(std::shared_ptr<Assign> expr) override;
    std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override;
    std::any visitCallExpr(std::shared_ptr<Call> expr) override;
    std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override;
    std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override;
    std::any visitLogicalExpr(std::shared_ptr<Logical> expr) override;
    std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override;
};
