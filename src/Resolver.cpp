# include "../include/Resolver.hpp"

Resolver::Resolver(Interpreter& interpreter) :
    interpreter{interpreter} {}

// resolve a statement
// std::vector<std::map<Token, int>> identifiers; 
void Resolver::resolve(std::shared_ptr<Statement::Stmt> statement) {
    statement->accept(*this);
}

// resolve an expression
void Resolver::resolve(std::shared_ptr<Expr> expression) {
    expression->accept(*this);
}

void Resolver::beginScope() {
    identifiers.push_back(std::map<Token, int>{});
    scopes.push_back(std::map<std::string, bool>{});
}

void Resolver::endScope() {
    identifiers.pop_back();
    scopes.pop_back();
}

void Resolver::declare(Token& name) {
    if (scopes.empty() && identifiers.empty()) return;
    std::map<std::string, bool> &currentScope = scopes.back();
    std::map<Token, int> &currentBlock = identifiers.back();
    if (currentScope.find(name.text) != currentScope.end()) {
        Error::log(name, "Multiple variables with same name not allowed.");
    }
    currentBlock[name] = 0;
    currentScope[name.text] = false;
}

void Resolver::define(Token &name) {
    if (scopes.empty() && identifiers.empty()) return;
    scopes.back()[name.text] = true;
}

void Resolver::resolveLocal(std::shared_ptr<Expr> expr, Token& name) {
    int scopeSize = scopes.size() - 1;
    for (int i = scopeSize; i >= 0; --i) {
        // if variable found in scope i
        if (scopes[i].find(name.text) != scopes[i].end()) {
            identifiers[i].erase(name);
            interpreter.resolve(expr, scopeSize - i);
            return;
        }
    }
}

void Resolver::resolveFunction(std::vector<Token> params,
                               std::vector<std::shared_ptr<Statement::Stmt>> body,
                               FType type) {
    FType enclosingFunction = currentFunction;
    currentFunction = type;
    beginScope();
    for (Token& param : params) {
        declare(param);
        define(param);
    }
    resolve(body);
    checkUnusedVariables();
    endScope();
    currentFunction = enclosingFunction;
}
                                
// resolve a vector of statements
void Resolver::resolve(std::vector<std::shared_ptr<Statement::Stmt>>&
                       statements) {
    for (std::shared_ptr<Statement::Stmt>& statement : statements) {
        resolve(statement);
    }
}

std::any Resolver::visitBlockStmt(std::shared_ptr<Statement::Block> stmt) {
    beginScope();
    resolve(stmt->statements);
    checkUnusedVariables();
    endScope();
    return {};
}

std::any Resolver::visitVarStmt(std::shared_ptr<Statement::Var> stmt) {
    declare(stmt->name);
    if (stmt->init != nullptr) resolve(stmt->init);
    define(stmt->name);
    return {};
}

std::any Resolver::visitFunctionStmt(std::shared_ptr<Statement::Function> stmt) {
    declare(stmt->name);
    define(stmt->name);
    resolveFunction(stmt->params, stmt->body, FType::FUNCTION);
    return {};
}

std::any Resolver::visitExpressionStmt(std::shared_ptr<Statement::Expression> stmt) {
    resolve(stmt->expression);
    return {};
}

std::any Resolver::visitIfStmt(std::shared_ptr<Statement::If> stmt) {
    resolve(stmt->condition);
    resolve(stmt->thenBranch);
    if (stmt->elseBranch != nullptr) resolve(stmt->elseBranch);
    return {};
}

std::any Resolver::visitPrintStmt(std::shared_ptr<Statement::Print> stmt) {
    resolve(stmt->expression);
    return {};
}

std::any Resolver::visitReturnStmt(std::shared_ptr<Statement::Return> stmt) {
    if (currentFunction == FType::NONE) {
        Error::log(stmt->keyword, "Can't return from top level code.");
    }
    if (stmt->value != nullptr) resolve(stmt->value);
    return {};
}

std::any Resolver::visitWhileStmt(std::shared_ptr<Statement::While> stmt) {
    resolve(stmt->condition);
    resolve(stmt->body);
    return {};
}

// accessing values of a variable
std::any Resolver::visitVariableExpr(std::shared_ptr<Variable> expr) {
    if (!scopes.empty()) {
        auto& currentScope = scopes.back();
        auto elem = currentScope.find(expr->name.text);
        // variable declared already but not initialized
        // for cases like:
        // var b = "ball";
        // {
        //     var b = b;
        //     print b;
        // }
        if (elem != currentScope.end() && elem->second == false) {
            Error::log(expr->name,
                      "Can't read local variable in its own initializer.");
        }
    }
    resolveLocal(expr, expr->name);
    return {};
}

std::any Resolver::visitAssignExpr(std::shared_ptr<Assign> expr) {
    resolve(expr->value);
    resolveLocal(expr, expr->name);
    return {};
}

std::any Resolver::visitBinaryExpr(std::shared_ptr<Binary> expr) {
    resolve(expr->left);
    resolve(expr->right);
    return {};
}

std::any Resolver::visitCallExpr(std::shared_ptr<Call> expr) {
    resolve(expr->callee);
    for (std::shared_ptr<Expr>& argument : expr->arguments) {
        resolve(argument);
    }
    return {};
}

std::any Resolver::visitGroupingExpr(std::shared_ptr<Grouping> expr) {
    resolve(expr->expression);
    return {};
}

std::any Resolver::visitLiteralExpr(std::shared_ptr<Literal> expr) {
    return {};
}

std::any Resolver::visitLogicalExpr(std::shared_ptr<Logical> expr) {
    resolve(expr->left);
    resolve(expr->right);
    return {};
}

std::any Resolver::visitUnaryExpr(std::shared_ptr<Unary> expr) {
    resolve(expr->right);
    return {};
}

std::any Resolver::visitLambdaExpr(std::shared_ptr<Lambda> expr) {
    resolveFunction(expr->params, expr->body, FType::LAMBDA);
    return {};
}

std::any Resolver::visitListExpr(std::shared_ptr<List> expr) {
    for (std::shared_ptr<Expr> value : expr->values) {
        resolve(value);
    }
    return {};
}

std::any Resolver::visitSubscriptExpr(std::shared_ptr<Subscript> expr) {
    resolve(expr->name);
    resolve(expr->index);
    if (expr->value != nullptr) resolve(expr->value);
    return {};
}

void Resolver::checkUnusedVariables() {
    std::map<Token, int> &currentScope = identifiers.back();
    for (auto const& [key, val] : currentScope) {
        Error::warn(key, "Unused local variable.");
    }
}
