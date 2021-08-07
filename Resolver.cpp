# include "./Resolver.hpp"

Resolver::Resolver(Interpreter& interpreter) :
    interpreter{interpreter} {}

// resolve a statement
void Resolver::resolve(std::shared_ptr<Stmt> statement) {
    statement.accept(*this);
}

// resolve an expression
void Resolver::resolve(std::shared_ptr<Expr> expression) {
    expression.resolve(*this);
}

void Resolver::beginScope() {
    scopes.push_back(std::map<std::string, bool>{});
}

void Resolver::endScope() {
    scopes.pop_back();
}

void Resolver::declare(Token& name) {
    if (scopes.empty()) return;
    std::map<std::string, bool> &currentScope = scopes.back();
    currentScope[name.lexeme] = false;
}

void Resolver::define(Token &name) {
    if (scopes.empty()) return;
    scoeps.back()[name.lexeme] = true;
}

// resolve a vector of statements
void Resolver::resolve(std::vector<std::shared_ptr<Stmt>>& statements) {
    for (std::shared_ptr<Stmt>& statement : statements) {
        resolve(statement);
    }
}

std::any Resolver::visitBlockStmt(std::shared_ptr<Block> stmt) {
    beginScope();
    resolve(stmt->statements);
    endScope();
    return {};
}

std::any Resolver::visitVarStmt(std::shared_ptr<Var> stmt) {
    declare(stmt->name);
    if (stmt->init != nullptr) resolve(stmt->init);
    define(stmt->name);
    return {};
}
