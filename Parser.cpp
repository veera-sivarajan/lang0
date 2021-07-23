# include "./Parser.hpp"

Parser::Parser(const vector<Token> tokens) :
    tokens(tokens) {}

Token Parser::peek() {
    return tokens.at(current);
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::EOF_TOKEN;
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) current += 1;
    return previous();
}

// check if current type is equal to any of the argument type
bool Parser::match(vector<TokenType> types) {
    for (auto type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

std::shared_ptr<Expr> Parser::expression() {
    return equality();
}

std::shared_ptr<Expr> Parser::equality() {
    std::shared_ptr<Expr> expr = comparison();
    vector<TokenType> types;
    types.push_back(TokenType::BANG_EQUAL);
    types.push_back(TokenType::EQUAL_EQUAL);
    while (match(types)) {
        Token oper = previous();
        std::shared_ptr<Expr> right = comparison();
        expr = std::make_shared<Binary>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::comparison() {
    std::shared_ptr<Expr> expr = term();
    vector<TokenType> types;
    types.push_back(TokenType::GREATER);
    types.push_back(TokenType::GREATER_EQUAL);
    types.push_back(TokenType::LESS);
    types.push_back(TokenType::LESS_EQUAL);
    while (match(types)) {
        Token oper = previous();
        std::shared_ptr<Expr> right = term();
        expr = std::make_shared<Binary>(expr, oper, right);
    }
    return expr;
}

// Subtraction and Addition
std::shared_ptr<Expr> Parser::term() {
    std::shared_ptr<Expr> expr = factor();
    vector<TokenType> types;
    types.push_back(TokenType::MINUS);
    types.push_back(TokenType::PLUS);
    while (match(types)) {
        Token oper = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<Binary>(expr, oper, right);
    }
    return expr;
}

// Division and Multiplication
std::shared_ptr<Expr> Parser::factor() {
    std::shared_ptr<Expr> expr = unary();
    vector<TokenType> types;
    types.push_back(TokenType::SLASH);
    types.push_back(TokenType::STAR);
    while (match(types)) {
        Token oper = previous();
        std::shared_ptr<Expr> right = unary();
        expr = std::make_shared<Binary>(expr, oper, right);
    }
    return expr;
}
