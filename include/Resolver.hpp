# pragma once

# include <vector>
# include <map>
# include "./Interpreter.hpp"

class Resolver: public ExprVisitor, public Statement::StmtVisitor {
private:
    Interpreter& interpreter;
    std::vector<std::map<std::string, bool>> scopes;
    std::vector<std::map<Token, int>> identifiers; 
    enum class FType{
        NONE,
        FUNCTION,
        LAMBDA,
    };

    FType currentFunction = FType::NONE;
    
    void resolve(std::shared_ptr<Statement::Stmt> statement);
    void resolve(std::shared_ptr<Expr> expression);
    void beginScope();
    void endScope();
    void declare(Token& name);
    void define(Token& name);
    void resolveLocal(std::shared_ptr<Expr> expr, Token& name);
    void resolveFunction(std::vector<Token> params,
                         std::vector<std::shared_ptr<Statement::Stmt>> body,
                         FType type);
    void resolveLambda(std::shared_ptr<Lambda> expr);
    

public:
    Resolver(Interpreter& interpreter);
    void resolve(std::vector<std::shared_ptr<Statement::Stmt>>& statements);
    void checkUnusedVariables();
    
    std::any visitBlockStmt(std::shared_ptr<Statement::Block> stmt) override;
    std::any visitVarStmt(std::shared_ptr<Statement::Var> stmt) override;
    std::any visitFunctionStmt(std::shared_ptr<Statement::Function> stmt)
        override;
    std::any visitExpressionStmt(std::shared_ptr<Statement::Expression> stmt)
        override;
    std::any visitIfStmt(std::shared_ptr<Statement::If> stmt) override;
    std::any visitPrintStmt(std::shared_ptr<Statement::Print> stmt) override;
    std::any visitReturnStmt(std::shared_ptr<Statement::Return> stmt) override;
    std::any visitWhileStmt(std::shared_ptr<Statement::While> stmt) override;

    std::any visitVariableExpr(std::shared_ptr<Variable> expr) override;
    std::any visitAssignExpr(std::shared_ptr<Assign> expr) override;
    std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override;
    std::any visitCallExpr(std::shared_ptr<Call> expr) override;
    std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override;
    std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override;
    std::any visitLogicalExpr(std::shared_ptr<Logical> expr) override;
    std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override;
    std::any visitLambdaExpr(std::shared_ptr<Lambda> expr) override;
    std::any visitListExpr(std::shared_ptr<List> expr) override;
    std::any visitSubscriptExpr(std::shared_ptr<Subscript> expr) override;
};
