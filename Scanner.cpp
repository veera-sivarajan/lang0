# include "./Token.hpp"

vector<Token> Scanner::scanTokens() {
    while(!isAtEnd()) {
        // Beginning of next lexeme
        start = current;
        scanToken();
    }
    Token token(EOF_TOKEN, "", line);
    tokens.push_back(token);
    return tokens;
}

bool Scanner::isAtEnd() {
    return current >= source.length();
}

void Scanner::scanToken() {
    char c = advance();

    switch(c) {
    case '(': addToken(LEFT_PAREN); break;
    case ')': addToken(RIGHT_PAREN); break;
    case '{': addToken(LEFT_BRACE); break;
    case '}': addToken(RIGHT_BRACE); break;
    case ',': addToken(COMMA); break;
    case '.': addToken(DOT); break;
    case '-': addToken(MINUS); break;
    case '+': addToken(PLUS); break;
    case ';': addToken(SEMICOLON); break;
    case '*': addToken(STAR); break;
    }
}
    
