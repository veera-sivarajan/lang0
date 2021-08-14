# pragma once
struct Binary;
struct Grouping;
struct Literal;
struct Unary;
struct Variable;
struct Assign;
struct Logical;
struct Call;
struct Lambda;
struct List;
struct Subscript;

struct ExprVisitor {
    //pure virutal functions
    virtual std::any visitBinaryExpr(std::shared_ptr<Binary> expr) = 0;
    virtual std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) = 0;
    virtual std::any visitLiteralExpr(std::shared_ptr<Literal> expr) = 0;
    virtual std::any visitUnaryExpr(std::shared_ptr<Unary> expr) = 0;
    virtual std::any visitVariableExpr(std::shared_ptr<Variable> expr) = 0;
    virtual std::any visitAssignExpr(std::shared_ptr<Assign> expr) = 0;
    virtual std::any visitLogicalExpr(std::shared_ptr<Logical> expr) = 0;
    virtual std::any visitCallExpr(std::shared_ptr<Call> expr) = 0;
    virtual std::any visitLambdaExpr(std::shared_ptr<Lambda> expr) = 0;
    virtual std::any visitListExpr(std::shared_ptr<List> expr) = 0;
    virtual std::any visitSubscriptExpr(std::shared_ptr<Subscript> expr) = 0;
    virtual ~ExprVisitor() = default;
};

struct Expr {
    virtual std::any accept(ExprVisitor &visitor) = 0;
};

struct Block;
struct Expression;
struct Print;
struct Var;
struct If;
struct While;
struct Function;
struct Return;

struct StmtVisitor {
    virtual std::any visitBlockStmt(std::shared_ptr<Block> stmt) = 0;
    virtual std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) = 0;
    virtual std::any visitPrintStmt(std::shared_ptr<Print> stmt) = 0;
    virtual std::any visitVarStmt(std::shared_ptr<Var> stmt) = 0;
    virtual std::any visitIfStmt(std::shared_ptr<If> stmt) = 0;
    virtual std::any visitWhileStmt(std::shared_ptr<While> stmt) = 0;
    virtual std::any visitFunctionStmt(std::shared_ptr<Function> stmt) = 0;
    virtual std::any visitReturnStmt(std::shared_ptr<Return> stmt) = 0;
    virtual ~StmtVisitor() = default;
};

struct Stmt {
    virtual std::any accept(StmtVisitor &visitor) = 0;
};
