# pragma once

# include <vector>
# include <map>

class Resolver: public ExprVisitor, public StmtVisitor {
private:
    Interpreter& interpreter;
    std::vector<std::map<std::string, bool>> scopes;
    
    void resolve(std::shared_ptr<Stmt> statement);
    void resolve(std::shared_ptr<Expr> expression);
    void beginScope();
    void endScope();
    void declare(Token& name);
    void define(Token& name);

public:
    Resolver(Interpreter& interpreter);
    void resolve(std::vector<std::shared_ptr<Stmt>>& statements);
    
    std::any visitBlockStmt(std::shared_ptr<Block> stmt) override;
    std::any visitVarStmt(std::shared_ptr<Var> stmt) override;
};
