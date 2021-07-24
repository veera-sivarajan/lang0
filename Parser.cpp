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
bool Parser::match(vector<TokenType> types) {
    for (auto type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

std::shared_ptr<Expr> Parser::parse() {
    try {
        return expression();
    } catch (ParseError error) {
        return nullptr;
    }
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

std::shared_ptr<Expr> Parser::unary() {
    vector<TokenType> types;
    types.push_back(TokenType::BANG);
    types.push_back(TokenType::MINUS);
    if (match(types)) {
        Token oper = previous();
        std::shared_ptr<Expr> right = unary();
        return std::make_shared<Unary>(oper, right);
    }
    return primary();
}

std::shared_ptr<Expr> Parser::primary() {
    vector<TokenType> types, f_type, t_type, n_type, lit_types, left_paren;
    types.push_back(TokenType::FALSE); // 1
    types.push_back(TokenType::TRUE); // 2
    types.push_back(TokenType::NIL); // 3
    types.push_back(TokenType::NUMBER); // 4
    types.push_back(TokenType::STRING); // 5
    types.push_back(TokenType::LEFT_PAREN); // 6

    // slice the only necessary types
    // shitty code but it works
    f_type = vector<TokenType>(types.begin(), types.end() - 5);
    if (match(f_type))
        return std::make_shared<Literal>(false);

    t_type = vector<TokenType>(types.begin() + 1, types.end() - 4);
    if (match(t_type))
        return std::make_shared<Literal>(true);

    n_type = vector<TokenType>(types.begin() + 2, types.end() - 3);
    if (match(n_type))
        return std::make_shared<Literal>(nullptr);

    lit_types = vector<TokenType>(types.begin() + 3, types.end() - 1);
    if (match(lit_types))
        return std::make_shared<Literal>(previous().literal);

    left_paren = vector<TokenType>(types.begin() + 5, types.end());
    if (match(left_paren)) {
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
    Dlox::error(token, message);
    return ParseError{""};
}
