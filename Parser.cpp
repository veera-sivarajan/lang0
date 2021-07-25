# include "./Parser.hpp"

using std::cout;

Parser::Parser(const vector<Token> &tokens) :
    tokens{tokens} {}

void Parser::peek_print() {
    Token temp = tokens.at(current);
    cout << "peek_print() = ";
    temp.print();
}

Token Parser::peek() {
    return tokens.at(current);
}

Token Parser::previous() {
    return tokens.at(current - 1);
}

bool Parser::isAtEnd() {
    cout << "Checking isAtEnd()...\n";
    peek_print();
    bool result = peek().type == TokenType::EOF_TOKEN;
    cout << "isAtEnd() -> " << result << "\n";
    return result; 
}

bool Parser::check(TokenType type) {
    std::cout << "Checking type...\n";
    std::cout << "Type Arg: " << magic_enum::enum_name(type) << "\n";
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) {
        std::cout << "Advancing...\n";
        ++current;
    }
    return previous();
}

// check if current type is equal to any of the argument type
template<class... T>
bool Parser::match(T... types) {
    std::cout << "Parser matching...\n";
    assert((... && std::is_same_v<T, TokenType>));
    if ((... || check(types))) {
        std::cout << "Type matched...\n";
        advance();
        return true;
    }
    return false;
}

std::shared_ptr<Expr> Parser::parse() {
    try {
        std::cout << "Parsing source code....\n";
        return expression();
    } catch (ParseError &error) {
        return nullptr;
    }
}

std::shared_ptr<Expr> Parser::expression() {
    cout << "Parsing expression...\n";
    return equality();
}

std::shared_ptr<Expr> Parser::equality() {
    cout << "Parsing equality...\n";
    std::shared_ptr<Expr> expr = comparison();
    cout << "Parsing equality done. \n";
    while (match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
        Token oper = previous();
        std::shared_ptr<Expr> right = comparison();
        expr = std::make_shared<Binary>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::comparison() {
    cout << "Parsing comparison...\n";
    std::shared_ptr<Expr> expr = term();
    cout << "Parsing term done. \n";
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
    cout << "Parsing term...\n";
    std::shared_ptr<Expr> expr = factor();
    cout << "Parsing factor done. \n";
    while (match(TokenType::MINUS, TokenType::PLUS)) {
        Token oper = previous();
        std::shared_ptr<Expr> right = factor();
        expr = std::make_shared<Binary>(expr, oper, right);
    }
    return expr;
}

// Division and Multiplication
std::shared_ptr<Expr> Parser::factor() {
    cout << "Parsing factor...\n";
    std::shared_ptr<Expr> expr = unary();
    while (match(TokenType::SLASH, TokenType::STAR)) {
        Token oper = previous();
        std::shared_ptr<Expr> right = unary();
        expr = std::make_shared<Binary>(expr, oper, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::unary() {
    cout << "Parsing unary...\n";
    if (match(TokenType::BANG, TokenType::MINUS)) {
        Token oper = previous();
        std::shared_ptr<Expr> right = unary();
        return std::make_shared<Unary>(oper, right);
    }
    return primary();
}

std::shared_ptr<Expr> Parser::primary() {
    cout << "Parsing primary...\n";
    if (match(TokenType::FALSE)) return std::make_shared<Literal>(false);
    else if (match(TokenType::TRUE)) return std::make_shared<Literal>(true);
    else if (match(TokenType::NIL)) return std::make_shared<Literal>(nullptr);

    else if (match(TokenType::NUMBER, TokenType::STRING))
        return std::make_shared<Literal>(previous().literal);

    else if (match(TokenType::LEFT_PAREN)) {
        std::shared_ptr<Expr> expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return std::make_shared<Grouping>(expr);
    }
    // does not match any terminals
    throw error(peek(), "Expect expression.");
} 

Token Parser::consume(TokenType type, string message) {
    cout << "Consume token...\n";
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
