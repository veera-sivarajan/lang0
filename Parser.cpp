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
