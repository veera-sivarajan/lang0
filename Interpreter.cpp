# include "./Interpreter.hpp"

int Clock::arity() {
    return 0;
}

std::any Clock::call(Interpreter &interpreter,
                    std::vector<std::any> arguments) {
    auto ticks = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration<double>{ticks}.count() / 1000.0;
}

std::string Clock::toString() {
    return "<native fn>";
}

Interpreter::Interpreter() {
    global->define("clock", std::shared_ptr<Clock>{});
}

void Interpreter::checkNumberOperand(const Token &oper,
                                     const std::any &operand) {
    if (operand.type() == typeid(double)) return;
    throw RuntimeError{oper, "Operand must be a number."};
}

void Interpreter::checkNumberOperands(const Token &oper, const std::any &left,
                                      const std::any &right) {

    if (left.type() == typeid(double) && right.type() == typeid(double)) return;
    throw RuntimeError{oper, "Operand must be a number."};
}

bool Interpreter::isEqual(const std::any &left, const std::any &right) {
    if (left.type() == typeid(nullptr) && right.type() == typeid(nullptr)) {
        return true;
    }

    if (left.type() == typeid(nullptr)) {
        return false;
    }

    if (left.type() == typeid(double) && right.type() == typeid(double)) {
        return std::any_cast<double>(left) == std::any_cast<double>(right);
    }

    if (left.type() == typeid(std::string) &&
        right.type() == typeid(std::string)) {
        return std::any_cast<string>(left) == std::any_cast<string>(right);
    }

    if (left.type() == typeid(bool) && right.type() == typeid(bool)) {
        return std::any_cast<bool>(left) == std::any_cast<bool>(right);
    }

    return false;
}

std::any Interpreter::visitBinaryExpr(std::shared_ptr<Binary> expr) {
    std::any left = evaluate(expr->left);
    std::any right = evaluate(expr->right);

    switch (expr->oper.type) {

    case TokenType::EQUAL_EQUAL:
        return isEqual(left, right);

    case TokenType::BANG_EQUAL:
        return !isEqual(left, right);

    case TokenType::GREATER:
        checkNumberOperands(expr->oper, left, right);
        return std::any_cast<double>(left) > std::any_cast<double>(right);

    case TokenType::GREATER_EQUAL:
        checkNumberOperands(expr->oper, left, right);
        return std::any_cast<double>(left) >= std::any_cast<double>(right);

    case TokenType::LESS:
        checkNumberOperands(expr->oper, left, right);
        return std::any_cast<double>(left) < std::any_cast<double>(right);

    case TokenType::LESS_EQUAL:
        checkNumberOperands(expr->oper, left, right);
        return std::any_cast<double>(left) <= std::any_cast<double>(right);

    case TokenType::MINUS:
        checkNumberOperands(expr->oper, left, right);
        return std::any_cast<double>(left) - std::any_cast<double>(right);

    case TokenType::PLUS:
        if (left.type() == typeid(double) && right.type() == typeid(double)) {
            return std::any_cast<double>(left) + std::any_cast<double>(right);
        } 
        if (left.type() == typeid(string) && right.type() == typeid(string)) {
            return std::any_cast<string>(left) + std::any_cast<string>(right);
        }
        throw RuntimeError{expr->oper, "Operands not of same type."};
        
    case TokenType::SLASH:
        checkNumberOperands(expr->oper, left, right);
        return std::any_cast<double>(left) / std::any_cast<double>(right);

    case TokenType::STAR:
        checkNumberOperands(expr->oper, left, right);
        return std::any_cast<double>(left) * std::any_cast<double>(right);
    default:
        return {};
    }
}

std::any Interpreter::visitLiteralExpr(std::shared_ptr<Literal> expr) {
    return expr->value;
}

// In Dlox, any value other than nil and false is true
bool Interpreter::isTruthy(const std::any &object) {
    if (object.type() == typeid(nullptr)) return false;
    if (object.type() == typeid(bool)) {
        return std::any_cast<bool>(object);
    }
    return true;
}

std::any Interpreter::visitUnaryExpr(std::shared_ptr<Unary> expr) {
    std::any right = evaluate(expr->right);

    switch (expr->oper.type) {
    case TokenType::BANG:
        return !isTruthy(right);
    case TokenType::MINUS:
        checkNumberOperand(expr->oper, right);
        return -std::any_cast<double>(right);
    default:
        return {};
    }
}

std::string Interpreter::stringify(const std::any &object) {
    if (object.type() == typeid(nullptr)) return "nil";

    if (object.type() == typeid(double)) {
        std::string text = std::to_string(std::any_cast<double>(object));
        if (text[text.length() - 7] == '.' && text[text.length() - 6] == '0') {
            text = text.substr(0, text.length() - 7);
        }
        return text;
    }

    if (object.type() == typeid(std::string)) {
        return std::any_cast<std::string>(object);
    }

    if (object.type() == typeid(bool)) {
        if (std::any_cast<bool>(object)) {
            string result{"true"};
            return result;
        } else {
            string result{"false"};
            return result;
        }
    }
    if (object.type() == typeid(std::shared_ptr<DloxFunction>)) {
        std::shared_ptr<DloxCallable> function;
        function = std::any_cast<std::shared_ptr<DloxFunction>>(object);
        return function->toString();
    }
    if (object.type() == typeid(std::shared_ptr<LambdaFunction>)) {
        std::shared_ptr<LambdaFunction> function;
        function = std::any_cast<std::shared_ptr<LambdaFunction>>(object);
        return function->toString();
    }

    return "stringify: cannot recognize type";
}

std::any Interpreter::evaluate(std::shared_ptr<Expr> expr) {
    return expr->accept(*this);
}

std::any Interpreter::visitExpressionStmt(std::shared_ptr<Expression> stmt) {
    std::any value = evaluate(stmt->expression);
    return {};
}

std::any Interpreter::visitPrintStmt(std::shared_ptr<Print> stmt) {
    std::any value = evaluate(stmt->expression);
    std::cout << stringify(value) << "\n";
    return {};
}

void Interpreter::execute(std::shared_ptr<Stmt> statement) {
    statement->accept(*this);
}

void Interpreter::executeBlock(const std::vector<std::shared_ptr<Stmt>>
                               &statements, std::shared_ptr<Env> new_env) {
    std::cout << "Executing block...\n";
    std::shared_ptr<Env> previous = this->curr_env;
    std::cout << "Previous memory...\n";
    previous->printKeys();
    try {
        this->curr_env = new_env;
        this->curr_env->printKeys();
        for (const std::shared_ptr<Stmt> &statement : statements) {
            execute(statement);
        }
    } catch(...) {
        this->curr_env = previous;
        throw;
    }
    this->curr_env = previous;
}

// AST interpretation begins here
void Interpreter::interpret(std::vector<std::shared_ptr<Stmt>> &statements) {
    try {
        for (std::shared_ptr<Stmt> &statement : statements) {
            execute(statement);
        }
    } catch (RuntimeError &error) {
        Error::runtimeError(error);
    }
}

std::any Interpreter::visitGroupingExpr(std::shared_ptr<Grouping> expr) {
    return evaluate(expr->expression);
}

std::any Interpreter::visitBlockStmt(std::shared_ptr<Block> stmt) {
    executeBlock(stmt->statements, std::make_shared<Env>(curr_env)); 
    return {};
}

std::any Interpreter::visitVarStmt(std::shared_ptr<Var> stmt) {
    std::any value = nullptr;
    if (stmt->init != nullptr) {
        value = evaluate(stmt->init);
    }
    curr_env->define(stmt->name.text, std::move(value));
    return {};
}

std::any Interpreter::visitIfStmt(std::shared_ptr<If> stmt) {
    if (isTruthy(evaluate(stmt->condition))) {
        execute(stmt->thenBranch);
    } else if (stmt->elseBranch != nullptr) {
        execute(stmt->elseBranch);
    }
    return {};
}

std::any Interpreter::lookUpVariable(Token& name, std::shared_ptr<Expr> expr) {
    auto elem = locals.find(expr);
    if (elem != locals.end()) {
        int distance = elem->second;
        return curr_env->getAt(distance, name.text);
    } else {
        return global->get(name);
    }
}
    
std::any Interpreter::visitVariableExpr(std::shared_ptr<Variable> expr) {
    std::any value = lookUpVariable(expr->name, expr);
    if (value.type() == typeid(nullptr)) {
        throw RuntimeError{expr->name, "Variable not initialized."};
    }
    return value;
}

std::any Interpreter::visitAssignExpr(std::shared_ptr<Assign> expr) {
    std::any value = evaluate(expr->value);
    auto elem = locals.find(expr);
    if (elem != locals.end()) {
        int distance = elem->second;
        curr_env->assignAt(distance, expr->name, value);
    } else {
        global->assign(expr->name, value);
    }
    return value;
}

std::any Interpreter::visitLogicalExpr(std::shared_ptr<Logical> expr) {
    std::any left = evaluate(expr->left);
    if (expr->oper.type == TokenType::OR) {
        if (isTruthy(left)) return left; // or is true if one exp is true
    } else {
        if (!isTruthy(left)) return left; // and is false if one exp is false
    }
    return evaluate(expr->right);
}

std::any Interpreter::visitWhileStmt(std::shared_ptr<While> stmt) {
    while (isTruthy(evaluate(stmt->condition))) {
        execute(stmt->body);
    }
    return {};
}

// Interpret function calls
std::any Interpreter::visitCallExpr(std::shared_ptr<Call> expr) {
    std::any callee = evaluate(expr->callee); // name of the function

    std::vector<std::any> arguments;
    for (std::shared_ptr<Expr> &argument : expr->arguments) {
        arguments.push_back(evaluate(argument));
    }

    std::shared_ptr<DloxCallable> function;
    if (callee.type() == typeid(std::shared_ptr<DloxFunction>)) {
        function = std::any_cast<std::shared_ptr<DloxFunction>>(callee);
    } else if (callee.type() == typeid(std::shared_ptr<LambdaFunction>)) {
        std::cout << "Casting to Lambda Function...\n";
        function = std::any_cast<std::shared_ptr<LambdaFunction>>(callee);
    } else {
        throw RuntimeError{expr->paren,
                "Can only call functions and classes."};
    }

    // NOTE: Checking arity
    if (static_cast<int>(arguments.size()) != function->arity()) {
        throw RuntimeError{expr->paren,
                "Expected " + std::to_string(function->arity()) +
                " arguments but got " + std::to_string(arguments.size()) + "."};
    }
    return function->call(*this, std::move(arguments));
}

// Interpreting function declaration
std::any Interpreter::visitFunctionStmt(std::shared_ptr<Function> stmt) {
    auto function = std::make_shared<DloxFunction>(stmt, curr_env);
    curr_env->define(stmt->name.text, function);
    return {};
}

std::any Interpreter::visitReturnStmt(std::shared_ptr<Return> stmt) {
    std::any value = nullptr;
    if (stmt->value != nullptr) value = evaluate(stmt->value);
    throw DloxReturn{value};
}

void Interpreter::resolve(std::shared_ptr<Expr> expr, int depth) {
    locals[expr] = depth;
}

std::any Interpreter::visitLambdaExpr(std::shared_ptr<Lambda> expr) {
    return std::make_shared<LambdaFunction>(expr, curr_env);
}
