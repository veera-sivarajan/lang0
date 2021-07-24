# include "./Parser.hpp"

Parser::Parser(const vector<Token> tokens) :
    tokens(tokens) {}

Token Parser::peek() {
    return tokens.at(current);
}

Token Parser::previous() {
    return tokens.at(current - 1);
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
template<class... T>
bool Parser::match(T... types) {
    if ((... || check(types))) {
        advance();
        return true;
    }
    return false;
}

std::shared_ptr<Expr> Parser::parse() {
    try {
        return expression();
    } catch (ParseError &error) {
        return nullptr;
    }
}

std::shared_ptr<Expr> Parser::expression() {
    return equality();
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
