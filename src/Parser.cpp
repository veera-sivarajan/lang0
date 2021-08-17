# include "../include/Parser.hpp"

using std::cout;

Parser::Parser(const vector<Token> &tokens) :
    tokens{tokens} {}

void Parser::peek_print() {
    Token temp = tokens.at(current);
    temp.print();
}

Token Parser::peek() {
    return tokens.at(current);
}

Token Parser::previous() {
    return tokens.at(current - 1);
}

bool Parser::isAtEnd() {
    bool result = peek().type == TokenType::EOF_TOKEN;
    return result; 
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) {
        ++current;
    }
    return previous();
}

// check if current type is equal to any of the argument type
template<class... T>
bool Parser::match(T... types) {
    assert((... && std::is_same_v<T, TokenType>));
    if ((... || check(types))) {
        advance();
        return true;
    }
    return false;
}

std::shared_ptr<Statement::Stmt> Parser::printStatement() {
    std::shared_ptr<Expr> value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_shared<Statement::Print>(value);
}

// what is expression statement?
std::shared_ptr<Statement::Stmt> Parser::expressionStatement() {
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_shared<Statement::Expression>(expr);
}

std::vector<std::shared_ptr<Statement::Stmt>> Parser::block() {
    std::vector<std::shared_ptr<Statement::Stmt>> statements;
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }
    consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

std::shared_ptr<Statement::Stmt> Parser::ifStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    std::shared_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");
    std::shared_ptr<Statement::Stmt> thenBranch = statement();
    std::shared_ptr<Statement::Stmt> elseBranch = nullptr;
    if (match(TokenType::ELSE)) {
        elseBranch = statement();
    }
    return std::make_shared<Statement::If>(condition, thenBranch, elseBranch);
}

std::shared_ptr<Statement::Stmt> Parser::whileStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    std::shared_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
    std::shared_ptr<Statement::Stmt> body = statement();
    return std::make_shared<Statement::While>(condition, body);
}

std::shared_ptr<Statement::Stmt> Parser::forStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");
    
    std::shared_ptr<Statement::Stmt> init;
    if (match(TokenType::SEMICOLON)) {
        init = nullptr;
    } else if (match(TokenType::VAR)) {
        init = varDeclaration();
    } else {
        init = expressionStatement();
    }

    std::shared_ptr<Expr> condition = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        condition = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");

    std::shared_ptr<Expr> increment = nullptr;
    if (!check(TokenType::RIGHT_PAREN)) {
        increment = expression();
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");

    std::shared_ptr<Statement::Stmt> body = statement();

    if (increment != nullptr) {
        body = std::make_shared<Statement::Block>(
            std::vector<std::shared_ptr<Statement::Stmt>>{
                body, std::make_shared<Statement::Expression>(increment)});
    }

    if (condition == nullptr) {
        condition = std::make_shared<Literal>(true);
    }
    body = std::make_shared<Statement::While>(condition, body);

    if (init != nullptr) {
        body = std::make_shared<Statement::Block>(
            std::vector<std::shared_ptr<Statement::Stmt>>{init, body});
    }
    return body;
}

std::shared_ptr<Statement::Stmt> Parser::returnStatement() {
    Token keyword = previous();
    std::shared_ptr<Expr> value = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        value = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after return value.");
    return std::make_shared<Statement::Return>(keyword, value);
}

std::shared_ptr<Statement::Stmt> Parser::statement() {
    if (match(TokenType::FOR)) return forStatement();
    if (match(TokenType::IF)) return ifStatement();
    if (match(TokenType::PRINT)) return printStatement();
    if (match(TokenType::RETURN)) return returnStatement();
    if (match(TokenType::WHILE)) return whileStatement();
    if (match(TokenType::LEFT_BRACE)) return std::make_shared<Statement::Block>(block());
    return expressionStatement();
}

std::shared_ptr<Statement::Stmt> Parser::varDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");

    std::shared_ptr<Expr> init = nullptr;
    if (match(TokenType::EQUAL)) {
        init = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<Statement::Var>(name, init);
}

// Parsing function declaration
std::shared_ptr<Statement::Function> Parser::function(std::string kind) {
    Token funcName = consume(TokenType::IDENTIFIER, "Expect " + kind + " name.");
    consume(TokenType::LEFT_PAREN, "Expect '(' after " + kind + " name.");
    std::vector<Token> parameters;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (parameters.size() >= 255) {
                error(peek(), "Can't have more than 255 parameters.");
            }
            parameters.push_back(
                consume(TokenType::IDENTIFIER, "Expect parameter name."));
        } while (match(TokenType::COMMA));
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");
    consume(TokenType::LEFT_BRACE, "Expect '{' before " + kind + " body.");
    std::vector<std::shared_ptr<Statement::Stmt>> body = block();
    return std::make_shared<Statement::Function>(std::move(funcName),
                                      std::move(parameters),
                                      std::move(body));
}

std::shared_ptr<Statement::Stmt> Parser::declaration() {
    try {
        if (match(TokenType::FUN)) return function("function");
        if (match(TokenType::VAR)) return varDeclaration();
        return statement();
    } catch (ParseError &error) {
        synchronize();
        return nullptr;
    }
}

std::vector<std::shared_ptr<Statement::Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Statement::Stmt>> statements;
    try {
        while (!isAtEnd()) {
            statements.push_back(declaration());
        }
    } catch (ParseError &error) {
        
    }
    return statements;
}

std::shared_ptr<Expr> Parser::logicalAnd() {
    std::shared_ptr<Expr> expr = equality();
    while (match(TokenType::AND)) {
        Token oper = previous();
        std::shared_ptr<Expr> right = equality();
        expr = std::make_shared<Logical>(expr, std::move(oper), right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::logicalOr() {
    std::shared_ptr<Expr> expr = logicalAnd();
    while (match(TokenType::OR)) {
        Token oper = previous();
        std::shared_ptr<Expr> right = logicalAnd();
        expr = std::make_shared<Logical>(expr, std::move(oper), right);
    }
    return expr;
}

// c = b = d = 4; 
std::shared_ptr<Expr> Parser::assignment() {
    std::shared_ptr<Expr> expr = logicalOr();
    if (match(TokenType::EQUAL)) {
        Token equals = previous();
        std::shared_ptr<Expr> value = assignment();
        if (Variable *e = dynamic_cast<Variable *>(expr.get())) {
            Token name = e->name;
            return std::make_shared<Assign>(std::move(name), value);
        } else if (Subscript *s = dynamic_cast<Subscript *>(expr.get())) {
            std::shared_ptr<Expr> name = s->name;
            std::shared_ptr<Expr> index = s->index;
            return std::make_shared<Subscript>(name, index, value, s->paren);
        }
        error(std::move(equals), "Invalid assignment target.");
    }
    return expr;
}

std::shared_ptr<Expr> Parser::expression() {
    return assignment();
}

std::shared_ptr<Expr> Parser::equality() {
    std::shared_ptr<Expr> expr = comparison();
    while (match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
        Token oper = previous();
        std::shared_ptr<Expr> right = comparison();
        expr = std::make_shared<Binary>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::comparison() {
    std::shared_ptr<Expr> expr = term();
    while (match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS,
                 TokenType::LESS_EQUAL)) {
        Token oper = previous();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<Binary>(expr, oper, right);
    }
    return expr;
}

// Subtraction and Addition
std::shared_ptr<Expr> Parser::term() {
    std::shared_ptr<Expr> expr = factor();
    while (match(TokenType::MINUS, TokenType::PLUS)) {
        Token oper = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<Binary>(expr, oper, right);
    }
    return expr;
}

// Division and Multiplication
std::shared_ptr<Expr> Parser::factor() {
    std::shared_ptr<Expr> expr = unary();
    while (match(TokenType::SLASH, TokenType::STAR)) {
        Token oper = previous();
        std::shared_ptr<Expr> right = unary();
        expr = std::make_shared<Binary>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::finishCall(std::shared_ptr<Expr> callee) {
    std::vector<std::shared_ptr<Expr>> arguments;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (arguments.size() >= 255) {
                error(peek(), "Can't have more than 255 arguments.");
            }
            arguments.push_back(expression());
        } while (match(TokenType::COMMA));
    }
    Token paren = consume(TokenType::RIGHT_PAREN, "Expect ')' after arguments.");
    return std::make_shared<Call>(callee, paren, arguments);
}

std::shared_ptr<Expr> Parser::finishSubscript(std::shared_ptr<Expr> name) {
    std::shared_ptr<Expr> index = logicalOr();
    Token paren = consume(TokenType::RIGHT_BRACKET,
                          "Expect ']' after arguments.");
    return std::make_shared<Subscript>(name, index, nullptr, paren);
}

std::shared_ptr<Expr> Parser::subscript() {
    std::shared_ptr<Expr> expr = primary();
    while (true) {
        if (match(TokenType::LEFT_BRACKET)) {
            expr = finishSubscript(expr);
        } else {
            break;
        }
    }
    return expr;
}

std::shared_ptr<Expr> Parser::call() {
    std::shared_ptr<Expr> expr = subscript(); // parses the callee
    while (true) {  // to support currying
        if (match(TokenType::LEFT_PAREN)) {
            expr = finishCall(expr);
        } else {
            break;
        }
    }
    return expr;
}

std::shared_ptr<Expr> Parser::unary() {
    if (match(TokenType::BANG, TokenType::MINUS)) {
        Token oper = previous();
        std::shared_ptr<Expr> right = unary();
        return std::make_shared<Unary>(oper, right);
    }
    return call();
}

// lambda (param1, param2) { return param1 * param2; }
std::shared_ptr<Expr> Parser::lambda() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'lambda'.");
    std::vector<Token> parameters;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (parameters.size() >= 255) {
                error(peek(), "Can't have more than 255 parameters.");
            }
            parameters.push_back(
                consume(TokenType::IDENTIFIER, "Expect parameter name."));
        } while (match(TokenType::COMMA));
    }
    consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");
    consume(TokenType::LEFT_BRACE, "Expect '{' before lambda body.");
    std::vector<std::shared_ptr<Statement::Stmt>> body = block();
    return std::make_shared<Lambda>(std::move(parameters), std::move(body));
}

std::shared_ptr<Expr> Parser::list() {
    std::vector<std::shared_ptr<Expr>> values = {};
    if (match(TokenType::RIGHT_BRACKET)) {
        return std::make_shared<List>(values);
    } else {
        do {
            if (values.size() >= 255) {
                error(peek(), "Can't have more than 255 elements in a list.");
            }
            std::shared_ptr<Expr> value = logicalOr();
            values.push_back(value);
        } while (match(TokenType::COMMA));
    }
    consume(TokenType::RIGHT_BRACKET, "Expect ']' at end of list.");
    return std::make_shared<List>(values);
}
    
std::shared_ptr<Expr> Parser::primary() {
    if (match(TokenType::LEFT_BRACKET)) return list();
    if (match(TokenType::LAMBDA)) return lambda();
    if (match(TokenType::FALSE)) return std::make_shared<Literal>(false);
    if (match(TokenType::TRUE)) return std::make_shared<Literal>(true);
    if (match(TokenType::NIL)) return std::make_shared<Literal>(nullptr);
    if (match(TokenType::IDENTIFIER))
        return std::make_shared<Variable>(previous());

    if (match(TokenType::NUMBER, TokenType::STRING))
        return std::make_shared<Literal>(previous().literal);

    // Groupings
    if (match(TokenType::LEFT_PAREN)) {
        std::shared_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<Grouping>(expr);
    }

    // does not match any terminals
    throw error(peek(), "Expect expression.");
} 

Token Parser::consume(TokenType type, string message) {
    if (check(type)) return advance();
    throw error(peek(), message);
}

void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        if (previous().type == TokenType::SEMICOLON) return;
        switch(peek().type) {
        case TokenType::CLASS:
        case TokenType::FUN:
        case TokenType::VAR:
        case TokenType::FOR:
        case TokenType::IF:
        case TokenType::WHILE:
        case TokenType::PRINT:
        case TokenType::RETURN:
        default:
            return;
        }
        advance();
    }
}

Parser::ParseError Parser::error(Token token, string message) {
    Error::log(token, message);
    return ParseError{""};
}
