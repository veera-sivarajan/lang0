# include "./Parser.hpp"

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

std::shared_ptr<Stmt> Parser::printStatement() {
    std::shared_ptr<Expr> value = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_shared<Print>(value);
}

// what is expression statement?
std::shared_ptr<Stmt> Parser::expressionStatement() {
    std::shared_ptr<Expr> expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after value.");
    return std::make_shared<Expression>(expr);
}

std::vector<std::shared_ptr<Stmt>> Parser::block() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }
    consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

std::shared_ptr<Stmt> Parser::ifStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
    std::shared_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");
    std::shared_ptr<Stmt> thenBranch = statement();
    std::shared_ptr<Stmt> elseBranch = nullptr;
    if (match(TokenType::ELSE)) {
        elseBranch = statement();
    }
    return std::make_shared<If>(condition, thenBranch, elseBranch);
}

std::shared_ptr<Stmt> Parser::whileStatement() {
    consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
    std::shared_ptr<Expr> condition = expression();
    consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");
    std::shared_ptr<Stmt> body = statement();
    return std::make_shared<While>(condition, body);
}

std::shared_ptr<Stmt> Parser::statement() {
    if (match(TokenType::IF)) return ifStatement();
    if (match(TokenType::PRINT)) return printStatement();
    if (match(TokenType::WHILE)) return whileStatement();
    if (match(TokenType::LEFT_BRACE)) return std::make_shared<Block>(block());
    return expressionStatement();
}

std::shared_ptr<Stmt> Parser::varDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");

    std::shared_ptr<Expr> init = nullptr;
    if (match(TokenType::EQUAL)) {
        init = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_shared<Var>(name, init);
}

std::shared_ptr<Stmt> Parser::declaration() {
    try {
        if (match(TokenType::VAR)) return varDeclaration();
        return statement();
    } catch (ParseError &error) {
        synchronize();
        return nullptr;
    }
}

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;
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

std::shared_ptr<Expr> Parser::unary() {
    if (match(TokenType::BANG, TokenType::MINUS)) {
        Token oper = previous();
        std::shared_ptr<Expr> right = unary();
        return std::make_shared<Unary>(oper, right);
    }
    return primary();
}

std::shared_ptr<Expr> Parser::primary() {
    if (match(TokenType::FALSE)) return std::make_shared<Literal>(false);
    if (match(TokenType::TRUE)) return std::make_shared<Literal>(true);
    if (match(TokenType::NIL)) return std::make_shared<Literal>(nullptr);
    if (match(TokenType::IDENTIFIER))
        return std::make_shared<Variable>(previous());

    if (match(TokenType::NUMBER, TokenType::STRING))
        return std::make_shared<Literal>(previous().literal);

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
